//
//  Cell.hpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//
#pragma once

class Cell {
public:
    Cell();
    ~Cell();
    
    int getX();
    int getY();
    int getDistance();
    bool getExplored();
    bool getTraversed();
    bool isWall(int dir);
    bool getWallInspected(int dir);
    Cell* getPrev();
    
    void setX(int _x);
    void setY(int _y);
    void setDistance(int _distance);
    void setExplored(bool _explored);
    void setTraversed(bool _traversed);
    void setWall(int _dir, bool _wall);
    void setWallInspected(int _dir, bool _inspected);
    void setPrev(Cell* _prev);
    
private:
    int x;
    int y;
    int distance;
    bool explored;
    bool traversed;
    bool walls[4];
    bool wallsInspected[4];
    Cell* prev;
};

