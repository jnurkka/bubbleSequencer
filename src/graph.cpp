//
//  graph.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "graph.hpp"
#include "bubble.hpp"



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

	int const BUBBLE_RADIUS = 10;

	int distNodes = (ofGetWindowWidth() - 2 * BUBBLE_RADIUS) / nr_nodes;
	int x = BUBBLE_RADIUS + distNodes / 2;

	for (int i = 0; i < nr_nodes; i += 1) {
		bubbles[i].init(x, ofGetWindowHeight() / 2, BUBBLE_RADIUS);
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
	int rectSize = 240;

	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() - rectSize, 0);

	ofSetHexColor(0xF3ECDB);
	ofDrawRectangle(0, 0, rectSize, rectSize);

	// Set the color for the text
	ofSetColor(0); // White text

	// Set the font size
	ofDrawBitmapString("Matrix:", 10, 20);

	// Set the initial position for drawing the matrix
	float startX = 40;
	float startY = 40;
	float spacing = 20; // Spacing between rows and columns

	// Loop through the matrix and draw each element
	for (int i = 0; i < adjMatrix.size(); ++i) {
		for (int j = 0; j < adjMatrix[i].size(); ++j) {
			// Convert float to string with a specified precision
			std::string valueStr = ofToString(int(adjMatrix[i][j]), 2);

			// Draw the value at the specified position
			if (bubbles[i].active) {
				ofDrawBitmapStringHighlight(valueStr, startX + j * spacing, startY + i * spacing);
			}
			else {
				ofDrawBitmapString(valueStr, startX + j * spacing, startY + i * spacing);
			}			
		}
	}

	ofPopMatrix();
}
