#include "ofApp.h"
#include "graph.hpp"

int const GRAPH_SIZE = 10;
Graph graph(GRAPH_SIZE);

//--------------------------------------------------------------
void ofApp::setup(){
	// Graph
    graph.init(GRAPH_SIZE);

	// BPM
	bpm.setBeatPerBar(4);
	bpm.setBpm(120);
	ofAddListener(bpm.beatEvent, this, &ofApp::triggerBeat);
	bpm.stop();

	// GUI
	gui.setup();
	button.addListener(this, &ofApp::button_gui_pressed);
	gui.add(intSlider.setup("BPM Slider", 120, ofxBpm::OFX_BPM_MIN, ofxBpm::OFX_BPM_MAX));
	gui.add(button.setup("Start/stop"));
}

//--------------------------------------------------------------
void ofApp::update(){
	// Update BPM based on GUI SLider
	bpm.setBpm(intSlider);
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw graph
    for (int i = 0; i < GRAPH_SIZE; i += 1) {
        ofSetHexColor(graph.bubbles[i].color);
        ofDrawCircle(graph.bubbles[i].xCoord, graph.bubbles[i].yCoord, graph.bubbles[i].rad);
    }

	// Draw GUI
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	// Kill BPM thread
	bpm.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	// Start / stop with Space
	if (key == ' ')
	{
		if (bpm.isPlaying())
		{
			bpm.stop();
		}
		else
		{
			bpm.start();
		}
	}
}

//--------------------------------------------------------------
void ofApp::triggerBeat()
{
    graph.activateNext();
}

//--------------------------------------------------------------
void ofApp::button_gui_pressed()
{
	if (bpm.isPlaying())
	{
		bpm.stop();
	}
	else
	{
		bpm.start();
	}
}

