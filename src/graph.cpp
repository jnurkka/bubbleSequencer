//
//  graph.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "graph.hpp"
#include "step.hpp"
#include <vector>

int const BUBBLE_RADIUS = 10;

Graph::Graph(int length, int height) : steps(length, height) {
    
}

void Graph::init(int length, int height) {
    int distNodes = (ofGetWindowWidth() - 2*BUBBLE_RADIUS) / length;
    int x = BUBBLE_RADIUS + distNodes / 2;
    for (int i = 0; i < length; i += 1) {
        steps[i].init(height, x, BUBBLE_RADIUS);
        x += distNodes;
    }
}

void Graph::activateNext() {
    int activeIndex = activeStep;
    if (activeIndex >= 0)
    {
        steps[activeIndex].deactivate();
    }
 
    if (activeIndex == steps.size() - 1) {
        steps[0].activate();
    } else {
        steps[activeIndex+1].activate();
    }
    if (activeIndex + 1 > steps.size() - 1) {
        activeStep = 0;
    } else {
        activeStep = activeIndex + 1;
    }
}

void Graph::draw() {
    for (int i = 0; i < steps.size(); i += 1) {
        steps[i].draw();
    }
}
