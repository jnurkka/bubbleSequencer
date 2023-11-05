//
//  step.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 05.11.23.
//

#include "step.hpp"
#include "bubble.hpp"
#include <tuple>


Step::Step(int size) : bubbles(size) {
    
}

void Step::init(int size, int xPos, int bubbleRadius) {
    using FilenameProbabilityPair = std::tuple<std::string, float>;
    std::vector<FilenameProbabilityPair> filenameProbabilities;
    filenameProbabilities.push_back(std::make_tuple("f.wav", 0.2f));
    filenameProbabilities.push_back(std::make_tuple("g.wav", 0.2f));
    filenameProbabilities.push_back(std::make_tuple("a.wav", 0.2f));
    filenameProbabilities.push_back(std::make_tuple("c.wav", 0.2f));
    filenameProbabilities.push_back(std::make_tuple("d.wav", 0.2f));
    
    int distNodes = (ofGetWindowHeight() - 2*bubbleRadius) / size;
    int y = bubbleRadius + distNodes / 2;
    for (int i = 0; i < size; i += 1) {
        bubbles[i].init(xPos, y, bubbleRadius, get<0>(filenameProbabilities[i]), get<1>(filenameProbabilities[i]));
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
