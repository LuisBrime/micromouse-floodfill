//
//  FloodFill.cpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//

#include "FloodFill.hpp"

#include <iostream>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <utility>
#include <vector>

#include "CellMod.hpp"
#include "API.h"

void FloodFill::solve() {
    std::cerr << "Starting maze." << std::endl;
    for (int a = 0; a < MAZE_SIZE_X; a += 1) {
        for (int b = 0; b < MAZE_SIZE_Y; b += 1) {
            cells[a][b].setX(a);
            cells[a][b].setY(b);
        }
    }
    
    floodFill();
}

void FloodFill::floodFill() {
    std::cerr << "Init." << std::endl;
    init();
    
    while (!inGoal(x, y)) {
        walls();
        flood(x, y);
        moveTowardsGoal();
    }
}

void FloodFill::init() {
    for (int _y = 0; y < MAZE_SIZE_Y/2; y++) {
        int dist = MAZE_SIZE_X/2 + MAZE_SIZE_Y/2 - 2 - y;
        for (int _x = 0; x < MAZE_SIZE_X; x++) {
            cells[_x][_y].setDistance(dist);
            cells[_x][MAZE_SIZE_Y-1-y].setDistance(dist);
            cells[MAZE_SIZE_X-1-x][MAZE_SIZE_Y-1-y].setDistance(dist);
            cells[MAZE_SIZE_X-1-x][y].setDistance(dist);
            dist--;
        }
    }
    
    for (int _y = 0; y < MAZE_SIZE_Y; y++) {
        for (int _x = 0; _x < MAZE_SIZE_X; x++) {
            cells[_x][_y].setWall(SOUTH, y == 0);
            cells[_x][_y].setWall(NORTH, y == MAZE_SIZE_Y-1);
            cells[_x][_y].setWall(WEST, x == 0);
            cells[_x][_y].setWall(EAST, x == MAZE_SIZE_X-1);
            
            cells[_x][_y].setWallInspected(SOUTH, y == 0);
            cells[_x][_y].setWallInspected(NORTH, y == MAZE_SIZE_Y-1);
            cells[_x][_y].setWallInspected(WEST, x == 0);
            cells[_x][_y].setWallInspected(EAST, x == MAZE_SIZE_X-1);
            
            cells[_x][_y].setPrev(NULL);
            cells[_x][_y].setExplored(false);
            cells[_x][_y].setTraversed(false);
        }
    }
    
    x = 0;
    y = 0;
    d = 0;
    steps = 0;
    explored = false;
    goal = false;
    checkpoint = true;
    history.init(SHORT_TERM_M, &cells[0][0]);
}

void FloodFill::walls() {
    cells[x][y].setWall(d, API::wallFront());
    cells[x][y].setWall((d+1)%4, API::wallRight());
    cells[x][y].setWall((d+3)%4, API::wallLeft());
    
    cells[x][y].setWallInspected(d, true);
    cells[x][y].setWallInspected((d+1)%4, true);
    cells[x][y].setWallInspected((d+3)%4, true);
    
    if (spaceFront()) {
        getFrontCell()->setWall((d+2)%4, API::wallFront());
        getFrontCell()->setWallInspected((d+2)%4, true);
        checkDeadEnd(getFrontCell());
    }
    if (spaceLeft()) {
        getLeftCell()->setWall((d+1)%4, API::wallLeft());
        getLeftCell()->setWallInspected((d+1)%4, true);
        checkDeadEnd(getLeftCell());
    }
    if (spaceRight()) {
        getRightCell()->setWall((d+3)%4, API::wallRight());
        getRightCell()->setWallInspected((d+3)%4, true);
        checkDeadEnd(getRightCell());
    }
}

void FloodFill::checkDeadEnd(Cell *cell) {
    int w = 0;
    for (int i = 0; i < 4; i++) {
        if (cell->isWall(i))
            w++;
    }
    
    if (w == 3) {
        for (int i = 0; i < 4; i++) {
            if (!cell->getWallInspected(i)) {
                cell->setWall(i, true);
                cell->setWallInspected(i, true);
                
                switch (i) {
                    case 0:
                        cells[cell->getX()][cell->getY()+1].setWall(2, true);
                        cells[cell->getX()][cell->getY()+1].setWallInspected(2, true);
                        checkDeadEnd(&cells[cell->getX()][cell->getY() + 1]);
                        break;
                    case 1:
                        cells[cell->getX()+1][cell->getY()].setWall(3, true);
                        cells[cell->getX()+1][cell->getY()].setWallInspected(3, true);
                        checkDeadEnd(&cells[cell->getX() + 1][cell->getY()]);
                        break;
                    case 2:
                        cells[cell->getX()][cell->getY()-1].setWall(0, true);
                        cells[cell->getX()][cell->getY()-1].setWallInspected(0, true);
                        checkDeadEnd(&cells[cell->getX()][cell->getY()-1]);
                    case 3:
                        cells[cell->getX()-1][cell->getY()].setWall(1, true);
                        cells[cell->getX()-1][cell->getY()].setWallInspected(1, true);
                        checkDeadEnd(&cells[cell->getX() - 1][cell->getY()]);
                        
                    default:
                        break;
                }
                break;
            }
        }
    }
}

void FloodFill::flood(int _x, int _y) {
    if (!inGoal(_x, _y)) {
        int northDist = MAZE_SIZE_X*MAZE_SIZE_Y;
        int eastDist = MAZE_SIZE_X*MAZE_SIZE_Y;
        int southDist = MAZE_SIZE_X*MAZE_SIZE_Y;
        int westDist = MAZE_SIZE_X*MAZE_SIZE_Y;
        
        if (!cells[_x][_y].isWall(NORTH)) {
            northDist = cells[_x][_y+1].getDistance();
        }
        if (!cells[_x][_y].isWall(EAST)) {
            eastDist = cells[_x+1][_y].getDistance();
        }
        if (!cells[_x][_y].isWall(SOUTH)) {
            southDist = cells[_x][_y-1].getDistance();
        }
        if (!cells[_x][_y].isWall(WEST)) {
            westDist = cells[_x-1][_y].getDistance();
        }
        
        if (cells[_x][_y].getDistance() != min(northDist, eastDist, southDist, westDist) + 1) {
            cells[_x][_y].setDistance(min(northDist, eastDist, southDist, westDist) + 1);
            
            if (!cells[_x][_y].isWall(NORTH))
                flood(x, y + 1);
            if (!cells[_x][_y].isWall(EAST))
                flood(x + 1, y);
            if (!cells[_x][_y].isWall(SOUTH))
                flood(x, y - 1);
            if (!cells[_x][_y].isWall(WEST))
                flood(x - 1, y);
        }
    }
}

void FloodFill::moveTowardsGoal() {
    int minDist = cells[x][y].getDistance();
    int relDir = 0;
    
    if (!cells[x][y].isWall((d+3)%4)) {
        if (getLeftCell()->getDistance() <= minDist) {
            minDist = getLeftCell()->getDistance();
            relDir = 3;
        }
    }
    
    if (!cells[x][y].isWall((d+2)%4)) {
        if (getRearCell()->getDistance() <= minDist) {
            minDist = getRearCell()->getDistance();
            relDir = 2;
        }
    }
    
    if (!cells[x][y].isWall((d+1)%4)) {
        if (getRightCell()->getDistance() <= minDist) {
            minDist = getRightCell()->getDistance();
            relDir = 1;
        }
    }
    
    if (!cells[x][y].isWall(d)) {
        if (getFrontCell()->getDistance() <= minDist) {
            minDist = getFrontCell()->getDistance();
            relDir = 0;
        }
    }
    
    switch (relDir) {
        case 1:
            turnRight();
            break;
        case 2:
            turnAround();
            break;
        case 3:
            turnLeft();
            break;
    }
    
    moveForward();
}

void FloodFill::moveForward() {
    switch (d) {
        case NORTH:
            y++;
            break;
        case EAST:
            x++;
            break;
        case SOUTH:
            y--;
            break;
        case WEST:
            x--;
            break;
    }
    
    API::moveForward();
    steps++;
    
    if (!explored && checkpoint)
        history.moved();
}

void FloodFill::turnRight() {
    d = (d + 1) % 4;
    API::turnRight();
    steps++;
}

void FloodFill::turnLeft() {
    d = (d + 3) % 4;
    API::turnLeft();
    steps++;
}

void FloodFill::turnAround() {
    d = (d + 2) % 4;
    API::turnAround();
    steps++;
}

bool FloodFill::inGoal(int _x, int _y) {
    bool h = (MAZE_SIZE_X - 1)/2 == x;
    if (MAZE_SIZE_X % 2 == 0) {
        h = h || (MAZE_SIZE_X) / 2 == x;
    }
    
    bool v = (MAZE_SIZE_Y - 1) / 2 == y;
    if (MAZE_SIZE_Y % 2 == 0) {
        v = v || (MAZE_SIZE_Y) / 2 == y;
    }
    
    return h && v;
}

int FloodFill::min(int o, int t, int th, int f) {
    int fM = o;
    int sM = t;
    
    if (t <= o)
        fM = t;
    if (f <= th)
        sM = f;
    if (fM <= sM)
        return fM;
    else
        return sM;
}

Cell* FloodFill::getFrontCell() {
    switch (d) {
        case NORTH:
            return &cells[x][y+1];
        case EAST:
            return &cells[x+1][y];
        case SOUTH:
            return &cells[x][y-1];
        case WEST:
            return &cells[x-1][y];
    }
    return NULL;
}

Cell* FloodFill::getLeftCell(){
    switch (d){
        case NORTH:
            return &cells[x-1][y];
        case EAST:
            return &cells[x][y+1];
        case SOUTH:
            return &cells[x+1][y];
        case WEST:
            return &cells[x][y-1];
    }
    return NULL;
}

Cell* FloodFill::getRightCell(){
    switch (d){
        case NORTH:
            return &cells[x+1][y];
        case EAST:
            return &cells[x][y-1];
        case SOUTH:
            return &cells[x-1][y];
        case WEST:
            return &cells[x][y+1];
    }
    return NULL;
}

Cell* FloodFill::getRearCell(){
    switch (d){
        case NORTH:
            return &cells[x][y-1];
        case EAST:
            return &cells[x-1][y];
        case SOUTH:
            return &cells[x][y+1];
        case WEST:
            return &cells[x+1][y];
    }
    return NULL;
}

bool FloodFill::spaceFront() {
    switch (d){
        case NORTH:
            return y + 1 < MAZE_SIZE_Y;
        case EAST:
            return x + 1 < MAZE_SIZE_X;
        case SOUTH:
            return y > 0;
        case WEST:
            return x > 0;
    }
    return false;
}

bool FloodFill::spaceLeft() {
    switch (d){
        case NORTH:
            return x > 0;
        case EAST:
            return y + 1 < MAZE_SIZE_Y;
        case SOUTH:
            return x + 1 < MAZE_SIZE_X;
        case WEST:
            return y > 0;
    }
    return false;
}

bool FloodFill::spaceRight() {
    switch (d){
        case NORTH:
            return x + 1 < MAZE_SIZE_X;
        case EAST:
            return y > 0;
        case SOUTH:
            return x > 0;
        case WEST:
            return y + 1 < MAZE_SIZE_Y;
    }
    return false;
}





