#include "ofApp.h"
#include "graph.hpp"

int const GRAPH_LENGTH = 10;
int const STEP_HEIGHT = 5;
Graph graph(GRAPH_LENGTH, STEP_HEIGHT);

//--------------------------------------------------------------
void ofApp::setup(){
	// Graph
    graph.init(GRAPH_LENGTH, STEP_HEIGHT);

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
	graph.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw graph
    graph.draw();

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

