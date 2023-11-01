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
    rad = 20;
    active = false;
    color = 0xFFFFFF;
}

void Bubble::init(int x, int y, int radius) {
    xCoord = x;
    yCoord = y;
    rad = radius;
}

void Bubble::activate() {
    active = true;
    color = 0x008000;
}

void Bubble::deactivate() {
    active = false;
    color = 0xFFFFFF;
}
