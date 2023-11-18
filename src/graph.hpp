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
        std::vector<std::vector<float>> adjMatrix; // [row][column]
        std::vector<int> levels;

        int activeStep = -1;
        int calculateNextStep();
        vector<int> findNextStepOptions();
    public:
        Graph(int size);
        ~Graph();

        int size();

        void addEdge(int source, int sink, float weight);
        void removeEdge(int source, int sink);

        void initLayout();

        void update();
        void updateLayout_SpringForces();

        void draw();
        void drawAdjMatrix();

        void activateNext();

        std::vector<Bubble> bubbles;

};
