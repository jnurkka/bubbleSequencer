//
//  graph.hpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//
#pragma once

#include "ofMain.h"
#include "step.hpp"
#include <vector>


class Graph {
    private:
        std::vector<Step> steps;
        int activeStep = -1;
    public:
        Graph(int length, int height);
        void init(int length, int height);
        void activateNext();
        void draw();
};
