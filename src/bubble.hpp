//
//  circle.hpp
//  creativeCoding_001
//
//  Created by Jaakko Nurkka on 01.11.23.
//
#pragma once

#include "ofMain.h"

class Bubble {
    public:
        int xCoord;
        int yCoord;
        int rad;
        Bubble();
        void init(int x, int y, int radius);
};
