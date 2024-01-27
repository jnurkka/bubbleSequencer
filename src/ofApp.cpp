#include "ofApp.h"
#include "graph.hpp"
#include "ambience.hpp"
#include "ColorManager.hpp"

int constexpr NR_BUBBLES = 18;
int constexpr MAX_CONNECTIONS = 3;

Graph graph = Graph(NR_BUBBLES);
Ambience ambience("ambience-river.mp3");

//--------------------------------------------------------------
void ofApp::setup(){

	// Open in fullscreen
	ofSetFullscreen(false);

	// Load font
	fontSize = 12;
	myFont.load("Lavigne.ttf", fontSize);
	
	// Graph	

	// Calc positions
    graph.initLayout(myFont);
	old_w = ofGetWidth();
	old_h = ofGetHeight();

	// Mouse control
	isLeftMouseDown = false;
	dragID = 0;

	// BPM
	int constexpr tempo = 120;
	bpm.setBeatPerBar(4);
	bpm.setBpm(tempo);
	ofAddListener(bpm.beatEvent, this, &ofApp::triggerBeat);
	bpm.stop();

	// GUI
	gui.setup();
	button.addListener(this, &ofApp::buttonGuiPressed);
	gui.add(int_slider.setup("BPM Slider", tempo, ofxBpm::OFX_BPM_MIN, ofxBpm::OFX_BPM_MAX));
	gui.add(f_slider_vol_ambi.setup("Ambience volume", 0.4, 0.0, 1.0));
	gui.add(button.setup("Start/stop"));
	gui.add(toggle_spring.setup("Spring Layout", true));
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

	// Update volume of ambience
	ambience.update_volume(f_slider_vol_ambi);

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

	// Background
	ofBackgroundGradient(ColorManager::getInstance().getColorBackground2(), ColorManager::getInstance().getColorBackground(), OF_GRADIENT_CIRCULAR);

	// Draw adj matrix
	if (!hide_adj_matrix) {
		graph.drawAdjMatrix();
	}
	
	// Draw graph
    graph.draw(stoi(bubbleId), !hide_adj_matrix);

	
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

		case 'f':
			ofToggleFullscreen();

		case 'r':
			graph.initRandom(NR_BUBBLES);


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
	// Calculate scaling factors based on the new window size
	float scaleX = (float)w / old_w;
	float scaleY = (float)h / old_h;

	// Update circle positions relative to the window size
	for (int i = 0; i < graph.bubbles.size(); i++) {
		graph.bubbles[i].pos.x = graph.bubbles[i].pos.x * scaleX;
		graph.bubbles[i].pos.y = graph.bubbles[i].pos.y * scaleY;
	}

	old_w = ofGetWidth();
	old_h = ofGetHeight();
}
