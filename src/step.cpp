//
//  step.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 05.11.23.
//

#include "step.hpp"
#include "bubble.hpp"

string const FILENAME[5] = {"f.wav", "g.wav", "a.wav", "c.wav", "d.wav" };

Step::Step(int size) : bubbles(size) {
    
}

void Step::init(int size, int xPos, int bubbleRadius) {
    int distNodes = (ofGetWindowHeight() - 2*bubbleRadius) / size;
    int y = bubbleRadius + distNodes / 2;
    for (int i = 0; i < size; i += 1) {
        string filename = FILENAME[int(ofRandom(FILENAME->size()))];
        bubbles[i].init(xPos, y, bubbleRadius, filename, 1);
        y += distNodes;
    }
}

void Step::deactivate() {
    for (int i = 0; i < bubbles.size(); i+= 1) {
        bubbles[i].deactivate();
    }
}

void Step::activate() {
    
}

void Step::draw() {
    for (int i = 0; i < bubbles.size(); i += 1) {
        bubbles[i].draw();
    }
}
