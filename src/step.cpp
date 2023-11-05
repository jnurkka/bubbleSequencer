//
//  step.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 05.11.23.
//

#include "step.hpp"
#include "bubble.hpp"
#include <random>
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
    // Calculate the total probability sum
    float totalProbability = 0.0f;
    for (const Bubble& bubble : bubbles) {
        totalProbability += bubble.probability;
    }

    // Generate a random number in the range [0, totalProbability)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, totalProbability);
    float randomValue = dis(gen);

    // Find the bubble to activate based on the random value
    float cumulativeProbability = 0.0f;
    for (Bubble& bubble : bubbles) {
        cumulativeProbability += bubble.probability;
        if (randomValue < cumulativeProbability) {
            bubble.activate();
            break;
        }
    }
}

void Step::draw() {
    for (int i = 0; i < bubbles.size(); i += 1) {
        bubbles[i].draw();
    }
}
