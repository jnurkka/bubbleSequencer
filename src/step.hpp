//
//  step.hpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 05.11.23.
//

#pragma once

#include "ofMain.h"
#include "bubble.hpp"
#include <vector>


class Step {
    private:
        std::vector<Bubble> bubbles;
    public:
        Step(int size);
        void init(int size, int xPos, int bubbleRadius);
        void draw();
        void activate();
        void deactivate();
        void update();
};
