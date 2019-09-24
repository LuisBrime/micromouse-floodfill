//
//  History.cpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/23/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//

#include "History.hpp"
#include <iostream>

void History::init(int _stm, Cell *origin) {
    stm = _stm;
    
    size = 0;
    while (!stacks.empty()) {
        stacks.pop();
    }
    while (!stackReferenceCounts.empty()) {
        stackReferenceCounts.pop_front();
    }
    while (!modifiedCells.empty()) {
        modifiedCells.pop_front();
    }
    
    std::stack<Cell*> tmp;
    tmp.push(origin);
    stacks.push(tmp);
    stackReferenceCounts.push_back(1);
    
    checkpointCell = origin;
    checkpointStack = tmp;
}

Cell* History::getCheckpointCell() { return checkpointCell; }

std::stack<Cell*> History::getCheckpointPath() {
    std::stack<Cell*> path;
    Cell* r = getCheckpointCell();
    
    while (r != NULL) {
        path.push(r);
        r = r->getPrev();
    }
    
    path.pop();
    return path;
}

std::stack<Cell*> History::getCheckpointStack() { return checkpointStack; }

void History::moved() {
    size++;
    
    if (stackReferenceCounts.empty() || stacks.empty()) {
        std::cout << "Error" << std::endl;
        exit(0);
    }
    
    int count = stackReferenceCounts.back();
    stackReferenceCounts.pop_back();
    stackReferenceCounts.push_back(count + 1);
    
    std::list<CellMod> e;
    modifiedCells.push_front(e);
    
    if (size > stm) {
        size--;
        int count2 = stackReferenceCounts.front();
        stackReferenceCounts.pop_front();
        stackReferenceCounts.push_front(count2 - 1);
    }
    if (stackReferenceCounts.front() == 0) {
        stackReferenceCounts.pop_front();
        stacks.pop();
        checkpointStack = stacks.front();
        if (stacks.front().top()->getPrev() != NULL) {
            checkpointCell = stacks.front().top()->getPrev();
        } else {
            std::cout << "Error" << std::endl;
            exit(0);
        }
    }
    
    if (modifiedCells.size() > stm) {
        modifiedCells.pop_back();
    }
}

void History::stackUpdate(std::stack<Cell *> _stack) {
    if (stackReferenceCounts.empty() || stacks.empty()) {
        std::cout << "Error" << std::endl;
        exit(0);
    }
    
    stacks.push(_stack);
    
    int tmp = stackReferenceCounts.back();
    stackReferenceCounts.pop_back();
    stackReferenceCounts.push_back(tmp - 1);
    stackReferenceCounts.push_back(1);
}

void History::modifiedCellsUpdate(std::list<CellMod> cells) {
    if (modifiedCells.size() > 0)
        modifiedCells.pop_front();
    modifiedCells.push_front(cells);
}

void History::reset() {
    std::list< std::list<CellMod> > tmp = modifiedCells;
    
    while (!tmp.empty()) {
        std::list<CellMod> cellList = tmp.front();
        tmp.pop_front();
        
        for (std::list<CellMod>::iterator it = cellList.begin(); it != cellList.end(); ++it) {
            (*it).cell->setPrev((*it).prev);
            (*it).cell->setDistance((*it).prevDist);
            (*it).cell->setExplored((*it).prevTraversed);
            (*it).cell->setTraversed((*it).prevTraversed);
            
            for (int i = 0; i < 4; i++) {
                (*it).cell->setWall(i, (*it).prevWalls[i]);
                (*it).cell->setWallInspected(i, (*it).prevWallsInspected[i]);
            }
        }
    }
    
    size = 0;
    while (!stacks.empty()) {
        stacks.pop();
    }
    while (!stackReferenceCounts.empty()) {
        stackReferenceCounts.pop_front();
    }
    while (!modifiedCells.empty()) {
        modifiedCells.pop_front();
    }
    
    stacks.push(checkpointStack);
    stackReferenceCounts.push_back(1);
}
