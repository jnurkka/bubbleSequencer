//
//  graph.hpp
//  bubbleSequencer
//
//
#pragma once

#include "ofMain.h"
#include "bubble.hpp"
#include <vector>
#include "ofTrueTypeFont.h"



class Graph {
    public:
        Graph();
        Graph(int size);
        ~Graph();

        int size();

        void initRandom(int size);

        void addEdge(int source, int sink, float weight);
        void removeEdge(int source, int sink);

        void initLayout(ofTrueTypeFont font);

        void update();
        void updateLayout_SpringForces();

        void draw(int selectedBubble, bool renderWeights);
        void drawAdjMatrix();

        void activateNext();

        std::vector<Bubble> bubbles;


    private:
        std::vector<std::vector<float>> adjMatrix;
        std::vector<int> levels;

        int activeStep = -1;
        int calculateNextStep();
        vector<tuple<int, float>> findNextStepOptions();

        // Font
        ofTrueTypeFont myFont;
};
