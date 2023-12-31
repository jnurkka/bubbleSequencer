//
//  graph.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "graph.hpp"
#include "bubble.hpp"



Graph::Graph(int size) : adjMatrix(size, std::vector<float>(size, 0.0f)), bubbles(size) {

}


Graph::~Graph() {

}


int Graph::size() {
	return bubbles.size();
}


void Graph::addEdge(const int source, const int sink, const float weight) {
	adjMatrix[source][sink] = weight;
}


void Graph::removeEdge(const int source, const int sink) {
	adjMatrix[source][sink] = 0.0f;
}


void Graph::initLayout(ofTrueTypeFont font) {
	// load font
	myFont = font;
	 
	// init all bubbles randomly
	for (int i = 0; i < bubbles.size(); i += 1) {
		bubbles[i].init(ofRandom(20.0f, ofGetWidth() - 20.0f), ofRandom(20.0f, ofGetHeight() - 20.0f), i, myFont);
	}
}

vector<tuple<int, float>> Graph::findNextStepOptions() {
    vector<tuple<int,float>> options;
    vector<float> nodes = adjMatrix[activeStep];
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i] > 0) {
            options.push_back(std::make_tuple(i, nodes[i]));
        }
    }
    return options;
}


int selectNodeFromOptions(vector<tuple<int, float>> options) {
    // Seed the random number generator with the current time
    ofSeedRandom();

    // Calculate the sum of all probabilities
    float sum = 0.0f;
    for (const auto& option : options) {
        sum += std::get<1>(option);
    }

    // Generate a random value between 0 and the sum of probabilities
    float randomValue = ofRandom(sum);

    // Find the option corresponding to the random value
    float cumulativeProbability = 0.0f;
    for (size_t i = 0; i < options.size(); ++i) {
        cumulativeProbability += std::get<1>(options[i]);
        if (randomValue < cumulativeProbability) {
            return get<0>(options[i]); // Return the ID of the selected option
        }
    }

    // if no next node option exists, return -1
    return -1;
}


int Graph::calculateNextStep() {
    int const activeIndex = activeStep;

    // if any of steps active, deactivate it
    if (activeIndex >= 0)
    {
        bubbles[activeIndex].deactivate();
    }

    if (activeIndex == -1) {
        // if no step active, activate first step
        return 0;
    }
    // othewise fetch next step options
    vector<tuple<int, float>> options = findNextStepOptions();
    
    int const nextIndex = selectNodeFromOptions(options);
    if (nextIndex == -1) return 0;
    return nextIndex;
}


void Graph::activateNext() {
    int const nextStep = calculateNextStep();
    activeStep = nextStep;
    bubbles[nextStep].activate();
}


void Graph::update()
{
	for (int i = 0; i < bubbles.size(); i += 1) {
		bubbles[i].update();
	}
}


void Graph::updateLayout_SpringForces()
{
	float damping = 0.07f; // Damping factor to prevent oscillations
	float k = 0.3f; // Spring constant
	float repulsion = 200000; // Node repulsion strength
	float spring_length = 200;

	// Update velocities based on spring forces
	for (int i = 0; i < bubbles.size(); i++) {
		for (int j = 0; j < bubbles.size(); j++) {
			if (adjMatrix[i][j]) {
				ofVec2f direction = bubbles[j].pos - bubbles[i].pos;

				const float distance = std::max(1.0f, direction.length());
				const ofVec2f force = (direction / distance) * k * (distance - spring_length);

				bubbles[i].vel += force;
				bubbles[j].vel -= force;

			}
		}
	}

	// Update velocities based on node repulsion
	for (int i = 0; i < bubbles.size(); i++) {
		for (int j = 0; j < bubbles.size(); j++) {
			if (i != j) {

				ofVec2f direction = bubbles[j].pos - bubbles[i].pos;

				float const distance = std::max(1.0f, direction.length());
				const ofVec2f force = (direction / distance) * repulsion / (distance * distance);

				//bubbles[i].vel -= force;
				bubbles[i].vel -= force;
				bubbles[j].vel += force;
			}
		}
	}

	// Update positions based on velocities
	for (int i = 0; i < bubbles.size(); i++) {
		for (int j = 0; j < bubbles.size(); j++) {
			bubbles[i].vel *= damping;
			bubbles[i].pos += bubbles[i].vel;
			// Keep nodes within the window bounds
			bubbles[i].pos.x = std::max(20.0f, std::min(bubbles[i].pos.x, float(ofGetWidth()) - 20.0f));
			bubbles[i].pos.y = std::max(20.0f, std::min(bubbles[i].pos.y, float(ofGetHeight()) - 20.0f));

			bubbles[j].vel *= damping;
			bubbles[j].pos += bubbles[j].vel;
			// Keep nodes within the window bounds
			bubbles[j].pos.x = std::max(20.0f, std::min(bubbles[j].pos.x, float(ofGetWidth())));
			bubbles[j].pos.y = std::max(20.0f, std::min(bubbles[j].pos.y, float(ofGetHeight())));
		}
	}
}


void Graph::draw(int selectedBubble, bool renderWeights) {
	
	// Draw edges
	for (int i = 0; i < adjMatrix.size(); i++) {
		ofPushMatrix();
		for (int j = 0; j < adjMatrix[i].size(); j++) {
			if (adjMatrix[i][j]) {
				bool isSelfLoop = i == j;
				float weight = adjMatrix[i][j];
				float lineThickness = weight * 2;
				float arrowSize = weight * 10;
				ofSetHexColor(0xF3ECDB);

				// Calculate direction vector
				ofVec2f direction;
				if (isSelfLoop) {
					direction = ofVec2f(bubbles[i].radius_animated.val(), 0).normalize();
				}
				else {
					direction = (bubbles[j].pos - bubbles[i].pos).getNormalized();
				}
				ofVec2f arrowheadPos = bubbles[j].pos - direction * bubbles[j].radius_animated.val();

				// Draw self-loops
				if (isSelfLoop) {
					ofPath curvedArrow;
					curvedArrow.moveTo(bubbles[i].pos + direction * bubbles[i].radius_animated.val());

					ofVec2f cp1 = bubbles[i].pos + 3 * direction * bubbles[i].radius_animated.val() + ofVec2f(0, -3 * bubbles[i].radius_animated.val());
					ofVec2f cp2 = bubbles[i].pos - 4 * direction * bubbles[i].radius_animated.val() + ofVec2f(0, -3 * bubbles[i].radius_animated.val());

					curvedArrow.bezierTo(cp1, cp2, arrowheadPos + ofVec2f(-arrowSize, 0));
					curvedArrow.setFilled(false);
					curvedArrow.setStrokeWidth(lineThickness);
					curvedArrow.draw();
				}
				else {
					// Draw edges
					ofFill();
					ofSetLineWidth(lineThickness);
					ofDrawLine(bubbles[i].pos, bubbles[j].pos);
				}

				if (renderWeights == true) {
					// draw weight
					ofVec2f textPos = (bubbles[i].pos + bubbles[j].pos) / 2;
					if (isSelfLoop) {
						textPos = ofVec2f(bubbles[i].pos.x, (bubbles[i].pos - 4 * bubbles[i].radius_animated.val()).y);
					}
					myFont.drawString(ofToString(weight), textPos.x, textPos.y);
				}

				// Draw arrow
				float angle = atan2(direction.y, direction.x);
				ofPushMatrix();
				ofTranslate(arrowheadPos.x, arrowheadPos.y);
				ofRotate(ofRadToDeg(angle));
				ofDrawTriangle(0, 0, -arrowSize, -arrowSize * 0.5, -arrowSize, arrowSize * 0.5);
				ofPopMatrix();
			}
		}
		ofPopMatrix();
	}

	// Draw nodes
	for (int i = 0; i < adjMatrix.size(); i++) {
		for (int j = 0; j < adjMatrix[i].size(); j++) {
			ofFill();
			if (i == selectedBubble)
			{
				bubbles[i].draw(true); // GUI has current bubble selected
			}
			else {
				bubbles[i].draw(); // Other bubbles
			}
		}
	}
}


void Graph::drawAdjMatrix() {
    // set the initial position for drawing the matrix
    float constexpr textCellWidth = 7.5;
    int constexpr startX = textCellWidth * 2;
    int constexpr spacing = 40;
    int constexpr startY = spacing;
    
	// rect size
	int const rectSize = startX + (spacing + adjMatrix.size()) * textCellWidth;

	// translate to the top left corner of the rectangle
	ofPushMatrix();
	ofTranslate(ofGetWidth() - rectSize, 0);
	ofSetHexColor(0xF3ECDB);
	ofDrawRectangle(0, 0, rectSize, rectSize);

	// set the color for the text
	ofSetColor(0);
	myFont.drawString("Adjacency matrix:", 10, 20); 

	for (int i = 0; i < adjMatrix.size(); i++) {
		for (int j = 0; j < adjMatrix[i].size(); j++) {
        
			// Convert float to string with a specified precision
			std::string valueStr = ofToString(adjMatrix[i][j], 1);

			// Highlight the currently active source 
			if (bubbles[i].active) {
				ofSetColor(0);
				myFont.drawString(valueStr, startX + j * spacing, startY + i * spacing);
			}
			else {
				ofSetColor(150);
				myFont.drawString(valueStr, startX + j * spacing, startY + i * spacing);
			}			
		}
	}
	ofPopMatrix();
}
