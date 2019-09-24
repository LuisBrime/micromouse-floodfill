//
//  Cell.cpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//
#include <iostream>
#include "Cell.hpp"

Cell::Cell() : x(0), y(0), distance(0), explored(false), traversed(false), prev(NULL) {
    for (int i = 0; i < 4; i++) {
        walls[i] = false;
        wallsInspected[i] = false;
    }
}

Cell::~Cell() {
}

// Getters
int Cell::getX() { return x; }
int Cell::getY() { return y; }
int Cell::getDistance() { return distance; }
bool Cell::getExplored() { return explored; }
bool Cell::getTraversed() { return traversed; }
Cell* Cell::getPrev() { return prev; }
bool Cell::isWall(int dir) { return walls[dir]; }
bool Cell::getWallInspected(int dir) { return wallsInspected[dir]; }

// Setters
void Cell::setX(int _x) { x = _x; }
void Cell::setY(int _y) { y = _y; }
void Cell::setDistance(int _distance) { distance = _distance; }
void Cell::setExplored(bool _explored) { explored = _explored; }
void Cell::setTraversed(bool _traversed) { traversed = _traversed; }
void Cell::setWall(int _dir, bool _wall) { walls[_dir] = _wall; }
void Cell::setWallInspected(int _dir, bool _inspected) { wallsInspected[_dir] = _inspected; }
void Cell::setPrev(Cell* _prev) { prev = _prev; }
