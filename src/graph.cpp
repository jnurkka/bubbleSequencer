//
//  graph.cpp
//  bubbleSequencer
//
//

#include "graph.hpp"
#include "bubble.hpp"
#include "ColorManager.hpp"



Graph::Graph() {
}

Graph::Graph(int size) {

	// Init steps
	activeStep = -1;
	previousStep = -1;
	for (int i = 0; i < size; i++) {
		bubbles.push_back(Bubble());
		adjMatrix.push_back(vector<float>(size, 0.0f));
	}

	initRandomGraph(size);
}


Graph::~Graph() {
	// Avoid clipping
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles[i].sample.setVolume(0.0f);
		bubbles[i].sample.stop();
	}
	bubbles.clear();
}

void Graph::initRandomGraph(int size) {
	// Init random graph
	int constexpr MAX_CONNECTIONS = 3;

	// Reset adj matrix
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			adjMatrix[i][j] = 0.0f;
		}
	}
	// Init nodes
	for (int i = 0; i < size; i++)
	{

		// ensure each node has at least one edge to another node
		int randomTarget = std::rand() % (size);
		while (randomTarget <= i) {
			randomTarget = std::rand() % (size);
			if (i == (size - 1)) {
				break;
			}
		}
		addEdge(i, randomTarget, ofRandom(0.5f, 1.0f));
		// add more edges (up to max con -1)

		if (i == (size - 1)) {
			addEdge(i, 0, 1.0f);
		}
		else {
			for (int j = 0; j < std::rand() % (MAX_CONNECTIONS - 1); j++) {
				addEdge(i, i + std::rand() % (size - i), ofRandom(0.5f, 1.0f));
			}
		}
	}
	/*
	// Core chain
	for (int i = 0; i < size-1; i++)
	{
		addEdge(i, i + 1, 1.0f);

	}
	addEdge(size-1, 0, 1.0f);

	// add random chains
	for (int i = 0; i < std::rand() % (6); i++)
	{
		int startNode = std::rand() % (size);
		while (startNode < size)
		{

			int nextNode = (int)ofRandom(0, size);
			addEdge(startNode, nextNode, 1.0f);
			startNode = nextNode;
		}
	}*/
	
}

int const Graph::size() {
	return bubbles.size();
}


void Graph::addEdge(const int source, const int sink, const float weight) {
	adjMatrix[source][sink] = weight;
}


void Graph::removeEdge(const int source, const int sink) {
	adjMatrix[source][sink] = 0.0f;
}


void Graph::initLayout(ofTrueTypeFont font, ofTrueTypeFont font_adj) {
	// load font
	myFont = font;
	myFont_adj = font_adj;
	 
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
	//ofLogNotice("How many follow nodes?") << options.size();
    return options;
}


int selectNodeFromOptions(vector<tuple<int, float>> options) {
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

	deactivateGraph();

	// if no step active, activate first step
    if (activeStep == -1) { return 0;}

    // otherwise fetch next step options
    vector<tuple<int, float>> options = findNextStepOptions();
    
    int const nextIndex = selectNodeFromOptions(options);
	if (nextIndex == -1) { return 0; }
    return nextIndex;
}

/// This will calculate the next active bubble.
void Graph::activateNext() {	
	previousStep = activeStep;
	activeStep = calculateNextStep();
}


void Graph::playNext(bool usingMidi) {
	if (!usingMidi) {
		bubbles[activeStep].activate_sound();
	}
	bubbles[activeStep].set_active_animations();
}

/// if any of steps active, deactivate it
void Graph::deactivateGraph() {
	if (activeStep >= 0)
	{
		bubbles[activeStep].deactivate_sound();
		bubbles[activeStep].set_inactive_animations();
	}
}

/// update radius and colour animations
void Graph::update()
{
	for (int i = 0; i < bubbles.size(); i += 1) {
		bubbles[i].update();
	}
}


void Graph::updateLayout_SpringForces() {
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


void Graph::draw(int selectedBubble) {
	
	// Draw edges
	for (int i = 0; i < adjMatrix.size(); i++) {
		ofPushMatrix();
		for (int j = 0; j < adjMatrix[i].size(); j++) {
			if (adjMatrix[i][j]) {
				bool isSelfLoop = i == j;
				float weight = adjMatrix[i][j];
				float lineThickness = weight * 3;
				float arrowSize = weight * 10;
				ofSetColor(ColorManager::getInstance().getColorEdges());  // Edge Colour

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
    int constexpr spacing = 20;
    int constexpr startY = spacing;
    
	// rect size
	int const rectSize = startX + (spacing * adjMatrix.size());// *textCellWidth;

	// translate to the top left corner of the rectangle
	ofPushMatrix();
	ofTranslate(ofGetWidth() - rectSize, 0);
	ofSetHexColor(0xF3ECDB); // background of adjacency
	ofDrawRectangle(0, 0, rectSize, rectSize + spacing);

	// set the color for the text
	ofSetColor(0); // title color
	myFont_adj.drawString("Adjacency matrix:", 10, 20);
	ofPopMatrix();

	for (int i = 0; i < adjMatrix.size(); i++) {
		for (int j = 0; j < adjMatrix[i].size(); j++) {
			// translate to the top left corner of the rectangle
			ofPushMatrix();
			ofTranslate(ofGetWidth() - rectSize, 0);
        
			// Convert float to string with a specified precision
			std::string valueStr = ofToString(adjMatrix[i][j], 1);

			// Highlight the currently active source 
			if (bubbles[i].active) {
				ofSetColor(0); // active row
				myFont_adj.drawString(valueStr, startX + j * spacing, startY + (i + 1) * spacing);
			}
			else {
				ofSetColor(150); //inactive row
				myFont_adj.drawString(valueStr, startX + j * spacing, startY + (i +1) * spacing);
			}		
			ofPopMatrix();

			// draw weight
			if (adjMatrix[i][j]) {
				ofVec2f textPos = (bubbles[i].pos + bubbles[j].pos) / 2;
				//self loop)
				if (i == j) {
					textPos = ofVec2f(bubbles[i].pos.x, (bubbles[i].pos - 4 * bubbles[i].radius_animated.val()).y);
				}
				myFont.drawString(ofToString(valueStr), textPos.x, textPos.y);
			}
		}
	}
}


int const Graph::getActiveStep() {
	return activeStep;
}


int const Graph::getPreviousStep() {
	return previousStep;
}
