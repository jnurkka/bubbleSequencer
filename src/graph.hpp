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

        int const size();

        void initRandomGraph(int size);

        void addEdge(int source, int sink, float weight);
        void removeEdge(int source, int sink);

        void initLayout(ofTrueTypeFont font, ofTrueTypeFont font_adj);

        void update();
        void updateLayout_SpringForces();

        void draw(int selectedBubble);
        void drawAdjMatrix();

        void activateNext();
        void playNext(bool usingMidi);
        void deactivateGraph();

        std::vector<Bubble> bubbles;

        int const getActiveStep();
        int const getPreviousStep();


    private:
        std::vector<std::vector<float>> adjMatrix;
        std::vector<int> levels;

        int activeStep = -1;
        int previousStep = -1;
        int calculateNextStep();
        vector<tuple<int, float>> findNextStepOptions();

        // Font
        ofTrueTypeFont myFont;
        ofTrueTypeFont myFont_adj;
};
