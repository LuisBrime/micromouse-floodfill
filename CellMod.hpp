//
//  Cell.hpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//
#pragma once
struct CellMod {
    Cell* cell;
    
    Cell* prev;
    int prevDist;
    bool prevExplored;
    bool prevTraversed;
    bool prevWalls[4];
    bool prevWallsInspected[4];
};
