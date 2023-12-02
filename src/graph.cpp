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

void Graph::initLayout() {
	// init all bubbles randomly
	for (int i = 0; i < bubbles.size(); i += 1) {
		bubbles[i].init(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()), i);
	}

	// Only for directed graphs. Ignoring self-loops
	// Assign levels based on the depth of each node in the graph
	// This is a simple way to ensure that higher-level nodes are drawn above lower-level nodes
	levels.resize(adjMatrix.size(), 0);
	for (int i = 0; i < adjMatrix.size(); ++i) {
		for (int j = 0; j < adjMatrix[i].size(); ++j) {
			if (adjMatrix[i][j] == 1) {
				if (i==j) {
					continue; // ignore the self-loops
				}
				else {
					levels[j] = std::max(levels[j], levels[i] + 1);
				}
			}
		}
	}

	// Count how many bubbles there are per level
	std::map<int, int> levelCount;
	for (int i = 0; i < levels.size(); i++) {
		// If the element is not present in the map, insert it with count 1
		// Otherwise, increment the count
		levelCount[levels[i]]++;
	}

	// Calculate the positions of nodes based on their levels
	float marginX = 100;
	float marginY = 100;
	float nodeSpacing = (ofGetHeight() - 2 * marginY) / (*max_element(levels.begin(), levels.end()) + 1);

	for (const auto& pair : levelCount) {
		int level_nr = pair.first;
		int nr_bubbles = pair.second;
		int width_counter = 0;

		for (int i = 0; i < bubbles.size(); i++) {
			if (level_nr == levels[i])
				{
					bubbles[i].pos.x = marginX + levels[i] * nodeSpacing;

					int temp_y = (ofGetWindowHeight() - (nr_bubbles - 1) * nodeSpacing) / 2;
					bubbles[i].pos.y = temp_y + width_counter * nodeSpacing;
					width_counter++;
				}
		}

		
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
	float k = 0.03f; // Spring constant
	float repulsion = 10; // Node repulsion strength

	// Update velocities based on spring forces
	for (int i = 0; i < bubbles.size(); i++) {
		for (int j = 0; j < bubbles.size(); j++) {
			if (adjMatrix[i][j]) {
				ofVec2f direction = bubbles[j].pos - bubbles[i].pos;

				const float distance = std::max(1.0f, direction.length());
				const ofVec2f force = (direction / distance) * k * (distance - 100);

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

				bubbles[i].vel -= force;
			}
		}
	}

	// Update positions based on velocities
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles[i].vel *= damping;
		bubbles[i].pos += bubbles[i].vel;

		// Keep nodes within the window bounds
		bubbles[i].pos.x = std::max(0.0f, std::min(bubbles[i].pos.x, float(ofGetWindowHeight())));
		bubbles[i].pos.y = std::max(0.0f, std::min(bubbles[i].pos.y, float(ofGetWindowHeight())));
	}
}


void Graph::draw() {
	for (int i = 0; i < adjMatrix.size(); i++) {
		// Draw edges
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
                } else {
                    direction = (bubbles[j].pos - bubbles[i].pos).getNormalized();
                }
                ofVec2f arrowheadPos = bubbles[j].pos - direction * bubbles[j].radius_animated.val();
                
				// Draw self-loops
				if (isSelfLoop) {
					ofPath curvedArrow;
					curvedArrow.moveTo(bubbles[i].pos + direction * bubbles[i].radius_animated.val());
					
					ofVec2f cp1 = bubbles[i].pos + 3*direction * bubbles[i].radius_animated.val() + ofVec2f(0, -3 * bubbles[i].radius_animated.val());
					ofVec2f cp2 = bubbles[i].pos - 4*direction * bubbles[i].radius_animated.val() + ofVec2f(0, -3 * bubbles[i].radius_animated.val());

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
                
                // draw weight
                ofVec2f textPos = (bubbles[i].pos + bubbles[j].pos) / 2;
                if (isSelfLoop) {
                    textPos = ofVec2f(bubbles[i].pos.x, (bubbles[i].pos - 4 * bubbles[i].radius_animated.val()).y);
                }
                ofDrawBitmapStringHighlight(ofToString(weight), textPos.x, textPos.y, ofColor::darkGreen);
                
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

		// Draw nodes
		ofFill();
		bubbles[i].draw();
	}
}



void Graph::drawAdjMatrix() {
	// rect size
	int const rectSize = 240;

	// translate to the top left corner of the rectangle
	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() - rectSize, 0);
	ofSetHexColor(0xF3ECDB);
	ofDrawRectangle(0, 0, rectSize, rectSize);

	// set the color for the text
	ofSetColor(0);
	ofDrawBitmapString("Adjacency matrix:", 10, 20);

	for (int i = 0; i < adjMatrix.size(); i++) {
		for (int j = 0; j < adjMatrix[i].size(); j++) {

			// set the initial position for drawing the matrix
			float constexpr startX = 40;
			float constexpr startY = 40;
			float constexpr spacing = 20;

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
