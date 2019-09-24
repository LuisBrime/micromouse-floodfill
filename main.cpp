//
//  main.cpp
//  FloodFill
//
//  Created by Luis Eduardo Brime Gomez on 9/22/19.
//  Copyright © 2019 Luis Eduardo Brime Gomez. All rights reserved.
//

#include <iostream>
#include "FloodFill.hpp"


void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, const char * argv[]) {
    log("Running Flood Fill...");
    
    FloodFill* ff;
    ff->solve();
}
