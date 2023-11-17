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
        int nr_nodes;
        std::vector<std::vector<float>> adjMatrix;

        int activeStep = -1;
    public:
        Graph(int size);
        ~Graph();

        int size();
        void addEdge(Bubble source, Bubble sink);
        void removeEdge(Bubble source, Bubble sink);

        void calcLayout();

        void update();
        void draw();
        void drawAdjMatrix();
        void activateNext();
};
