//
//  graph.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "graph.hpp"
#include "bubble.hpp"
#include <vector>

int const BUBBLE_RADIUS = 10;

Graph::Graph(int size) : bubbles(size) {
    
}

void Graph::init(int numNodes) {
    int distNodes = (ofGetWindowWidth() - 2*BUBBLE_RADIUS) / numNodes;
    int y = ofGetWindowHeight() / 2;
    int x = BUBBLE_RADIUS + distNodes / 2;
    for (int i = 0; i < numNodes; i += 1) {
        bubbles[i].init(x, y, BUBBLE_RADIUS);
        x += distNodes;
    }
}

void Graph::activateNext() {
    int activeIndex = -1;
    for (int i = 0; i < bubbles.size(); i += 1) {
        if (bubbles[i].active == true) {
            activeIndex = i;
            break;
        }
    }
    if (activeIndex >= 0)
    {
        bubbles[activeIndex].deactivate();
    }
 
    if (activeIndex == bubbles.size() - 1) {
        bubbles[0].activate();
    } else {
        bubbles[activeIndex+1].activate();
    }

}
