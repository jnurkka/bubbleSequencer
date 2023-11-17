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


void Graph::addEdge(int source, int sink, float weight) {
	adjMatrix[source][sink] = weight;
}


void Graph::removeEdge(int source, int sink) {
	adjMatrix[source][sink] = 0.0f;
}

void Graph::calcLayout() {

	// init all bubbles randomly
	for (int i = 0; i < nr_nodes; i += 1) {
		bubbles[i].init(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()), i);
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


	for (int i = 0; i < adjMatrix.size(); i++) {
		ofPushMatrix();
		for (int j = 0; j < adjMatrix[i].size(); j++) {
			if (adjMatrix[i][j]) {
				ofSetHexColor(0xF3ECDB);
				if (i == j) {
					ofNoFill();
					ofDrawCircle(bubbles[i].xCoord, bubbles[i].yCoord - bubbles[i].rad, bubbles[i].rad);
				}
				else {
					ofDrawLine(bubbles[i].xCoord, bubbles[i].yCoord, bubbles[j].xCoord, bubbles[j].yCoord);
				}
			}
		}
		ofPopMatrix();
		ofFill();
		bubbles[i].draw();
	}
	
}

void Graph::drawAdjMatrix() {
	// rect size
	int rectSize = 240;

	// translate to the top left corner of the rectangle
	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() - rectSize, 0);
	ofSetHexColor(0xF3ECDB);
	ofDrawRectangle(0, 0, rectSize, rectSize);

	// set the color for the text
	ofSetColor(0);
	ofDrawBitmapString("Adjacency matrix:", 10, 20);

	// set the initial position for drawing the matrix
	float startX = 40;
	float startY = 40;
	float spacing = 20;

	for (int i = 0; i < adjMatrix.size(); i++) {
		for (int j = 0; j < adjMatrix[i].size(); j++) {
			// Convert float to string with a specified precision
			std::string valueStr = ofToString(int(adjMatrix[i][j]), 2);

			// Highlight the currently active source 
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
