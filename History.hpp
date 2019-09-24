//
//  History.hpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//

#pragma once

#include <list>
#include <queue>
#include <stack>
#include <utility>

#include "Cell.hpp"
#include "CellMod.hpp"

class History {
public:
    void init(int _stm, Cell* origin);
    
    Cell* getCheckpointCell();
    std::stack<Cell*> getCheckpointStack();
    std::stack<Cell*> getCheckpointPath();
    
    void moved();
    void stackUpdate(std::stack<Cell*> _stack);
    void modifiedCellsUpdate(std::list<CellMod> cells);
    
    void reset();
    
private:
    int stm;
    int size;
    Cell* checkpointCell;
    std::stack<Cell*> checkpointStack;
    std::queue< std::stack<Cell*> > stacks;
    std::list<int> stackReferenceCounts;
    std::list< std::list<CellMod> > modifiedCells;
};
