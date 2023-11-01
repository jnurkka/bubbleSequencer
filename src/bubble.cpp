//
//  bubble.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "bubble.hpp"

Bubble::Bubble() {
    xCoord = 100;
    yCoord = 100;
}

void Bubble::init(int x, int y) {
    xCoord = x;
    yCoord = y;
}
