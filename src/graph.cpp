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

void Graph::initLayout() {
	// init all bubbles randomly
	for (int i = 0; i < nr_nodes; i += 1) {
		bubbles[i].init(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()), i);
	}

	// Assuming the graph is acyclic and directed
	// Assign levels based on the depth of each node in the graph
	// This is a simple way to ensure that higher-level nodes are drawn above lower-level nodes
	levels.resize(adjMatrix.size(), 0);
	for (int i = 0; i < adjMatrix.size(); ++i) {
		for (int j = 0; j < adjMatrix[i].size(); ++j) {
			if (adjMatrix[i][j] == 1) {
				levels[j] = std::max(levels[j], levels[i] + 1);
			}
		}
	}

	// Calculate the positions of nodes based on their levels
	float marginX = 100;
	float marginY = 100;
	float layerSpacing = (ofGetWidth() - 2 * marginX) / (levels.size() + 1);
	float nodeSpacing = (ofGetHeight() - 2 * marginY) / (*max_element(levels.begin(), levels.end()) + 1);


	for (int i = 0; i < levels.size(); i++) {
		bubbles[i].pos_x = marginX + i * layerSpacing;
		bubbles[i].pos_y = marginY + levels[i] * nodeSpacing;
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


void Graph::updateLayout_SpringForces()
{
	float damping = 0.7; // Damping factor to prevent oscillations
	float k = 0.3; // Spring constant
	float repulsion = 100; // Node repulsion strength

	// Update velocities based on spring forces
	for (int i = 0; i < nr_nodes; i++) {
		for (int j = 0; j < nr_nodes; j++) {
			if (adjMatrix[i][j]) {
				float direction_x = bubbles[j].pos_x - bubbles[i].pos_x;
				float direction_y = bubbles[j].pos_y - bubbles[i].pos_y;

				float distance = std::max(1.0f, std::sqrt(direction_x * direction_x + direction_y * direction_y));

				float force_x = (direction_x / distance) * k * (distance - 100);
				float force_y = (direction_x / distance) * k * (distance - 100);

				bubbles[i].vel_x += force_x;
				bubbles[i].vel_y += force_y;

				bubbles[j].vel_x -= force_x;
				bubbles[j].vel_y -= force_y;
			}
		}
	}

	// Update velocities based on node repulsion
	// Update velocities based on spring forces
	for (int i = 0; i < nr_nodes; i++) {
		for (int j = 0; j < nr_nodes; j++) {
			if (i != j) {

				float direction_x = bubbles[j].pos_x - bubbles[i].pos_x;
				float direction_y = bubbles[j].pos_y - bubbles[i].pos_y;

				float distance = std::max(1.0f, std::sqrt(direction_x * direction_x + direction_y * direction_y));

				float force_x = (direction_x / distance) * repulsion / (distance * distance);
				float force_y = (direction_x / distance) * repulsion / (distance * distance);

				bubbles[i].vel_x -= force_x;
				bubbles[i].vel_y -= force_y;
			}
		}
	}

	// Update positions based on velocities
	for (int i = 0; i < nr_nodes; i++) {
		bubbles[i].vel_x *= damping;
		bubbles[i].vel_y *= damping;

		bubbles[i].pos_x += bubbles[i].vel_x;
		bubbles[i].pos_y += bubbles[i].vel_y;

		// Keep nodes within the window bounds
		bubbles[i].pos_x = std::max(0.0f, std::min(bubbles[i].pos_x, float(ofGetWindowWidth())));
		bubbles[i].pos_y = std::max(0.0f, std::min(bubbles[i].pos_y, float(ofGetWindowHeight())));

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
					ofDrawCircle(bubbles[i].pos_x, bubbles[i].pos_y - bubbles[i].rad, bubbles[i].rad);
				}
				else {
					ofDrawLine(bubbles[i].pos_x, bubbles[i].pos_y, bubbles[j].pos_x, bubbles[j].pos_y);
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
