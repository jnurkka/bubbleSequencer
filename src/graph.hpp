//
//  graph.hpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//
#pragma once

#include "ofMain.h"
#include "bubble.hpp"
#include <vector>


class Graph {
    private:
        std::vector<Bubble> bubbles;
        int activeStep = -1;
    public:
        Graph(int size);
        void init(int numNodes);
        void activateNext();
        void draw();
};
