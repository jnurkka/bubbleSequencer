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
    public:
        std::vector<Bubble> bubbles;
        Graph(int size);
        void init(int numNodes);
        void activateNext();
};
