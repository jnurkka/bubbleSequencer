#include "ofApp.h"
#include "graph.hpp"


int const NR_BUBBLES = 9;
Graph graph(NR_BUBBLES);

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofBackgroundHex(0x3E503C);
	// Graph   TODO: 1) manually define nodes. 2) make it based on CSV file and real data :)
	// Set edges
	graph.addEdge(0, 1, 1.0f);
	graph.addEdge(1, 2, 1.0f);
	graph.addEdge(2, 3, 1.0f);
	graph.addEdge(2, 4, 1.0f);
	graph.addEdge(3, 5, 1.0f);
	graph.addEdge(3, 6, 1.0f);
	graph.addEdge(4, 6, 1.0f);
	graph.addEdge(5, 5, 1.0f);
	graph.addEdge(5, 7, 1.0f);
	graph.addEdge(6, 7, 1.0f);
	graph.addEdge(6, 8, 1.0f);
	
	// Calc positions
    graph.calcLayout();

	// BPM
	int constexpr tempo = 20;
	bpm.setBeatPerBar(4);
	bpm.setBpm(tempo);
	ofAddListener(bpm.beatEvent, this, &ofApp::triggerBeat);
	bpm.stop();

	// GUI
	gui.setup();
	button.addListener(this, &ofApp::button_gui_pressed);
	gui.add(intSlider.setup("BPM Slider", tempo, ofxBpm::OFX_BPM_MIN, ofxBpm::OFX_BPM_MAX));
	gui.add(button.setup("Start/stop"));
}

//--------------------------------------------------------------
void ofApp::update(){
	// Update BPM based on GUI SLider
	bpm.setBpm(intSlider);

	// Update graph
	graph.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw graph
    graph.draw();
	graph.drawAdjMatrix();

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

