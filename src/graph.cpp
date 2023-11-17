//
//  graph.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "graph.hpp"
#include "bubble.hpp"
#include <vector>
#include <tuple>

int const BUBBLE_RADIUS = 10;

Graph::Graph(int size) : bubbles(size), adjMatrix(size, std::vector<float>(size, 0.0f)) {
	nr_nodes = size;
}


Graph::~Graph() {

}


int Graph::size() {
	return nr_nodes;
}


void Graph::addEdge(Bubble source, Bubble sink) {

}


void Graph::removeEdge(Bubble source, Bubble sink) {

}

void Graph::calcLayout() {
	int distNodes = (ofGetWindowWidth() - 2 * BUBBLE_RADIUS) / nr_nodes;
	int x = BUBBLE_RADIUS + distNodes / 2;

	for (int i = 0; i < nr_nodes; i += 1) {
		bubbles[i].init(x, ofGetWindowHeight() / 2, BUBBLE_RADIUS);    // TODO right filename / random nesss
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
	}
	else {
		bubbles[activeIndex + 1].activate();
	}
	if (activeIndex + 1 > bubbles.size() - 1) {
		activeStep = 0;
	}
	else {
		activeStep = activeIndex + 1;
	}
}

void Graph::update()
{
	for (int i = 0; i < bubbles.size(); i += 1) {
		bubbles[i].update();
	}
}

void Graph::draw() {
    for (int i = 0; i < bubbles.size(); i += 1) {
		bubbles[i].draw();
    }
}

void Graph::drawAdjMatrix() {

}
