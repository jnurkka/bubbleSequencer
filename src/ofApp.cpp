#include "ofApp.h"
#include "graph.hpp"
#include "ambience.hpp"

int constexpr NR_BUBBLES = 9;
Graph graph(NR_BUBBLES);
Ambience ambience("ambience-river.mp3");

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofBackgroundHex(0x3E503C);
	// Graph   TODO: 1) manually define nodes. 2) make it based on CSV file and real data :)
	// Set edges
	graph.addEdge(0, 1, 0.5f);
	graph.addEdge(0, 2, 0.5f);
	graph.addEdge(1, 2, 1.0f);
	graph.addEdge(2, 3, 0.5f);
	graph.addEdge(2, 4, 1.0f);
	graph.addEdge(3, 5, 1.0f);
	graph.addEdge(3, 6, 1.0f);
	graph.addEdge(3, 8, 1.0f);
	graph.addEdge(4, 6, 1.0f);
	graph.addEdge(5, 5, 1.0f);
	graph.addEdge(5, 7, 1.0f);
	graph.addEdge(6, 7, 1.0f);
	graph.addEdge(6, 8, 1.0f);
	
	// Calc positions
    graph.initLayout();

	// Mouse control
	isLeftMouseDown = false;
	dragID = 0;

	// BPM
	int constexpr tempo = 20;
	bpm.setBeatPerBar(4);
	bpm.setBpm(tempo);
	ofAddListener(bpm.beatEvent, this, &ofApp::triggerBeat);
	bpm.stop();

	// GUI
	gui.setup();
	button.addListener(this, &ofApp::buttonGuiPressed);
	gui.add(int_slider.setup("BPM Slider", tempo, ofxBpm::OFX_BPM_MIN, ofxBpm::OFX_BPM_MAX));
	gui.add(button.setup("Start/stop"));
	gui.add(toggle_spring.setup("Spring Layout", false));
	gui.add(hide_adj_matrix.setup("Hide Adj Matrix", true));

	// GUI bubbles
	gui.add(bubbleId.setup("Bubble ID", ofToString(graph.bubbles[0].bubbleID)));
	gui.add(bubbleFile.setup("Sample", graph.bubbles[0].file));

	bubbleNote.addListener(this, &ofApp::bubbleNoteChanged);
	gui.add(bubbleNote.setup("Midi note", graph.bubbles[0].midi_note, 0, 127));
}


//--------------------------------------------------------------
void ofApp::update(){
	// Update BPM based on GUI Slider
	bpm.setBpm(int_slider);

	// Update graph layout
	if (toggle_spring)
	{
		graph.updateLayout_SpringForces();
	}
	

	// Update bubbles
	graph.update();
}


//--------------------------------------------------------------
void ofApp::draw(){

	// Draw adj matrix
	if (!hide_adj_matrix) {
		graph.drawAdjMatrix();
	}
	
	// Draw graph
    graph.draw(!hide_adj_matrix);

	
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

	switch (key) {
		case ' ':
			if (bpm.isPlaying()){
				bpm.stop();
                ambience.pause();
			} else {
				bpm.start();
                ambience.play();
			}
			break;

		case OF_KEY_LEFT:
			int_slider = int_slider - 5;
			break;

		case OF_KEY_RIGHT:
			int_slider = int_slider + 5;
			break;

		default:
			break;
	}
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_LEFT) {
		for (int i = 0; i < graph.bubbles.size(); i++) {
			if (ofDist(x, y, graph.bubbles[i].pos.x, graph.bubbles[i].pos.y) < graph.bubbles[i].radius_animated.val()) {
				isLeftMouseDown = true;
				dragID = i;

				// update GUI
				bubbleId = ofToString(graph.bubbles[i].bubbleID);
				bubbleFile = graph.bubbles[i].file;
				bubbleNote = graph.bubbles[i].midi_note;
			}
		}
	}

}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (isLeftMouseDown) {
		graph.bubbles[dragID].pos.set(x, y);
	}
}



//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_LEFT) {
		isLeftMouseDown = false;
	}
}



//--------------------------------------------------------------
void ofApp::triggerBeat(){
    graph.activateNext();
}


//--------------------------------------------------------------
void ofApp::buttonGuiPressed(){
	if (bpm.isPlaying())
	{
		bpm.stop();
        ambience.pause();
	}
	else
	{
		bpm.start();
        ambience.play();
	}
}


//--------------------------------------------------------------
void ofApp::bubbleNoteChanged(int& midiNote) {
	graph.bubbles[stoi(bubbleId)].midi_note = midiNote;
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	for (int i = 0; i < graph.bubbles.size(); i++) {
		// Update circle positions relative to their original positions
		graph.bubbles[i].pos.x = graph.bubbles[i].pos.x * (float)w / ofGetWidth();
		graph.bubbles[i].pos.y = graph.bubbles[i].pos.y * (float)h / ofGetHeight();
	}
}