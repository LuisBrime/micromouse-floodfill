//
//  FloodFill.hpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//
#pragma once

#include <list>

#include "Cell.hpp"
#include "CellMod.hpp"
#include "History.hpp"

static const int MAZE_SIZE_X = 16;
static const int MAZE_SIZE_Y = 16;
static const int SHORT_TERM_M = 8;
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

class FloodFill {
public:
    void solve();
    
private:
    Cell cells[16][16];
    int y;
    int x;
    int d;
    int steps;
    bool goal;
    bool explored;
    History history;
    bool checkpoint;
    
    void init();
    void floodFill();
    
    void walls();
    void checkDeadEnd(Cell* cell);
    void flood(int _x, int _y);
    void moveTowardsGoal();
    bool inGoal(int _x, int _y);
    
    void moveForward();
    void turnRight();
    void turnLeft();
    void turnAround();
    
    Cell* getFrontCell();
    Cell* getLeftCell();
    Cell* getRightCell();
    Cell* getRearCell();
    bool spaceFront();
    bool spaceLeft();
    bool spaceRight();
    int min(int o, int t, int th, int f);
    
    void moveOneCell(Cell* target);
};

