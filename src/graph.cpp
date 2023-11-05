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
string const FILENAME[5] = {"f.wav", "g.wav", "a.wav", "c.wav", "d.wav" };


Graph::Graph(int size) : bubbles(size) {
    
}

void Graph::init(int numNodes) {
    int distNodes = (ofGetWindowWidth() - 2*BUBBLE_RADIUS) / numNodes;
    int y = ofGetWindowHeight() / 2;
    int x = BUBBLE_RADIUS + distNodes / 2;
    for (int i = 0; i < numNodes; i += 1) {
        string filename = FILENAME[int(ofRandom(FILENAME->size()))];
        bubbles[i].init(x, y, BUBBLE_RADIUS, filename, 1);
        // cout << bubbles[i].file << endl;
        x += distNodes;
    }
}

void Graph::activateNext() {
    int activeIndex = activeStep;
    if (activeIndex >= 0)
    {
        bubbles[activeIndex].deactivate();
    }
 
    if (activeIndex == bubbles.size() - 1) {
        bubbles[0].activate();
    } else {
        bubbles[activeIndex+1].activate();
    }
    if (activeIndex + 1 > bubbles.size() - 1) {
        activeStep = 0;
    } else {
        activeStep = activeIndex + 1;
    }
}

void Graph::draw() {
    for (int i = 0; i < bubbles.size(); i += 1) {
        bubbles[i].draw();
    }
}
