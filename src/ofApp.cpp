#include "ofApp.h"
#include "graph.hpp"
#include "ambience.hpp"
#include "ColorManager.hpp"

int constexpr NR_BUBBLES = 18;
int constexpr MAX_CONNECTIONS = 3;

bool const USE_MIDI = true; // TODO

// asdfGraph graph = Graph(NR_BUBBLES);
Ambience ambience("ambience-river.mp3");

//--------------------------------------------------------------
void ofApp::setup(){

	// Open in fullscreen
	ofSetFullscreen(false);

	// Load font
	fontSize = 12;
	myFont.load("Lavigne.ttf", fontSize);
	
	// Graph
	graph = Graph(NR_BUBBLES);	

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

	// GUI keys
	gui.add(label_space.setup("Space", "Start / stop"));
	gui.add(label_arrows.setup("Arrows", "Adjust tempo"));
	gui.add(label_reset.setup("r", "Reset graph"));
	gui.add(label_full.setup("f", "Fullscreen"));


	// GUI bubbles
	gui.add(bubbleId.setup("Bubble ID", ofToString(graph.bubbles[0].bubbleID)));
	gui.add(bubbleFile.setup("Sample", graph.bubbles[0].file));

	bubbleNote.addListener(this, &ofApp::bubbleNoteChanged);
	gui.add(bubbleNote.setup("Midi note", graph.bubbles[0].midi_note, 0, 127));


	// Init Midi
	if (USE_MIDI) {
		// Debug
		ofSetLogLevel(OF_LOG_VERBOSE);
		
		// print the available output ports to the console
		midiOut.listOutPorts();

		// connect
		midiOut.openPort(1); // by number
		//midiOut.openPort("IAC Driver Pure Data In"); // by name
		//midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port

	}
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

	// Debug Midi
	if (USE_MIDI) {
		// Check it is connected
		stringstream text;
		text << "connected to port " << midiOut.getPort()
			<< " \"" << midiOut.getName() << "\"" << endl
			<< "is virtual?: " << midiOut.isVirtual() << endl << endl;
		ofDrawBitmapString(text.str(), 20, 20);
	}
}


//--------------------------------------------------------------
void ofApp::exit(){
	// Kill BPM thread
	bpm.stop();

	// Close midi port
	if (USE_MIDI) {
		midiOut.closePort(); // TODO: send final midi �ff
	}
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
			break;

		case 'r':
			graph = Graph((std::rand() % 20) + 5);
			graph.initLayout(myFont);

			// update GUI
			dragID = 0;
			bubbleId = ofToString(graph.bubbles[0].bubbleID);
			bubbleFile = graph.bubbles[0].file;
			bubbleNote = graph.bubbles[0].midi_note;
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

	// MIDI
	if (USE_MIDI) {
		int const activeStep = graph.getActiveStep();
		int const previousStep = graph.getPreviousStep();

		midiOut.sendNoteOn(1, graph.bubbles[activeStep].midi_note, 127);
		if (previousStep != -1) {
			midiOut.sendNoteOff(1, graph.bubbles[previousStep].midi_note, 64); // TODO also send off for looping notes
		}
		// print out both the midi note and the frequency
		ofLogNotice() << "note: " << graph.bubbles[activeStep].midi_note
			<< " freq: " << ofxMidi::mtof(graph.bubbles[activeStep].midi_note) << " Hz";
	}
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
