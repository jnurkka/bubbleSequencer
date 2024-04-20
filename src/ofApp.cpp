#include "ofApp.h"
#include "graph.hpp"
#include "ambience.hpp"
#include "ColorManager.hpp"

int constexpr NR_BUBBLES = 18;
int constexpr MAX_CONNECTIONS = 3;

bool const USE_MIDI = false; // TODO make this accessible from GUI and link it to triggering the sound. 

// asdfGraph graph = Graph(NR_BUBBLES);
Ambience ambience("ambience-river.mp3");

//--------------------------------------------------------------
void ofApp::setup(){

	// Open in fullscreen
	ofSetFullscreen(false);
	ofSetFrameRate(60);

	// Load font
	fontSize = 12;
	myFont.load("Lavigne.ttf", fontSize);
	myFont_adj.load("Lavigne.ttf", 8);
	
	// Graph
	graph = Graph(NR_BUBBLES);	

	// Calc positions
	graph.initLayout(myFont, myFont_adj);
	old_w = ofGetWidth();
	old_h = ofGetHeight();

	// Mouse control
	isLeftMouseDown = false;
	dragID = 0;

	// Ableton Link
	abletonLinkThread.setup();
	ofAddListener(abletonLinkThread.newBeatEvent, this, &ofApp::triggerBeat);
	metronomeCircle = false;

	ofAddListener(abletonLinkThread.abletonLink.bpmChanged, this, &ofApp::bpmChanged);
	ofAddListener(abletonLinkThread.abletonLink.numPeersChanged, this, &ofApp::numPeersChanged);
	ofAddListener(abletonLinkThread.abletonLink.playStateChanged, this, &ofApp::playStateChanged);
	//lastBeat = -1.0;
	//currentBeat = abletonLinkThread.abletonLink.getBeat();
	//isPlaying = abletonLinkThread.abletonLink.isPlaying();
	
	// GUI
	gui.setup();
	button.addListener(this, &ofApp::buttonGuiPressed);
	gui.add(int_slider.setup("Tempo Slider", abletonLinkThread.abletonLink.getBPM(), 32, 420));
	gui.add(f_slider_vol_ambi.setup("Ambience volume", 0.4, 0.0, 1.0));
	gui.add(button.setup("Start/stop"));
	gui.add(toggle_spring.setup("Spring Layout", true));
	gui.add(show_adj_matrix.setup("Show Adj Matrix", false));

	// GUI keys
	gui.add(label_space.setup("Space", "Start / stop"));
	gui.add(label_arrows.setup("Arrows", "Adjust tempo"));
	gui.add(label_reset.setup("r", "Reset graph"));
	gui.add(label_full.setup("f", "Fullscreen"));


	// GUI bubbles
	gui.add(selected_bubble_id.setup("Bubble ID", ofToString(graph.bubbles[0].bubbleID)));
	gui.add(bubbleFile.setup("Sample", graph.bubbles[0].file));

	bubbleNote.addListener(this, &ofApp::bubbleNoteChanged);
	gui.add(bubbleNote.setup("Midi note", graph.bubbles[0].midi_note, 0, 127));


	// Init Midi
	if (USE_MIDI) {
		// print the available output ports to the console
		midiOut.listOutPorts();

		// connect  TODO: make it possible to select this from GUI dropdown
		midiOut.openPort(1); // by number
		//midiOut.openPort("IAC Driver Pure Data In"); // by name
		//midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port

		// GUI debug
		gui.add(label_midi_port.setup("Midi Port", to_string(midiOut.getPort())));
		gui.add(label_midi_name.setup("Midi Name", midiOut.getName()));
		gui.add(label_midi_virtual.setup("Midi isVirtual", to_string(midiOut.isVirtual())));
	}
	else {
		gui.add(label_midi_port.setup("Midi Port", "No Midi. Using internal sound."));
	}

	// Debug
	lastUpdateTime_update = ofGetElapsedTimeMillis() ;
	lastUpdateTime_draw = ofGetElapsedTimeMillis();;

}


//--------------------------------------------------------------
void ofApp::update(){
	//// Debug time
	//long currentTime = ofGetElapsedTimeMillis();
	//long elapsedTime = currentTime - lastUpdateTime_update;
	//lastUpdateTime_update = currentTime;
	//ofLogNotice("time for update") << elapsedTime;

	// Update BPM based on GUI Slider
	abletonLinkThread.abletonLink.setBPM(int_slider);

	// Update volume of ambience
	ambience.update_volume(f_slider_vol_ambi);

	// Update graph layout
	if (toggle_spring) {graph.updateLayout_SpringForces();}

	// Update bubbles radius and colour animation
	graph.update();
}


//--------------------------------------------------------------
void ofApp::draw(){
	//// Debug time
	//long currentTime = ofGetElapsedTimeMillis();
	//long elapsedTime = currentTime - lastUpdateTime_draw;
	//lastUpdateTime_draw = currentTime;
	//ofLogNotice("time for draw") << elapsedTime;

	// Background
	ofBackgroundGradient(ColorManager::getInstance().getColorBackground2(), ColorManager::getInstance().getColorBackground(), OF_GRADIENT_CIRCULAR);

	// Draw graph. Plot all bubbles and highlight the selected bubble
	graph.draw(stoi(selected_bubble_id));

	// Draw metronome circles
	ofSetColor(255);
	if(metronomeCircle) {
		ofDrawCircle(30, ofGetHeight() - 30, 6);
	}
	else {
		ofDrawCircle(50, ofGetHeight() - 30, 6);
	}
	
	// Draw GUI
	gui.draw();

	// Debug Ableton
	std::stringstream ss("");
	ss << "fps:   " << ofGetFrameRate() << std::endl
	   << "bpm:   " << abletonLinkThread.abletonLink.getBPM() << std::endl
	   << "beat:  " << abletonLinkThread.abletonLink.getBeat() << std::endl
	   << "phase: " << abletonLinkThread.abletonLink.getPhase() << std::endl
	   << "peers: " << abletonLinkThread.abletonLink.getNumPeers() << std::endl
	   << "play?: " << (abletonLinkThread.abletonLink.isPlaying() ? "play" : "stop");
	ofSetColor(255);
	ofDrawBitmapString(ss.str(), 20, ofGetHeight() - 130);

	// Optional: Draw adj matrix
	if (show_adj_matrix) { graph.drawAdjMatrix();}
}


//--------------------------------------------------------------
void ofApp::exit(){

	// Remove listeners
	ofRemoveListener(abletonLinkThread.newBeatEvent, this, &ofApp::triggerBeat);
	ofRemoveListener(abletonLinkThread.abletonLink.bpmChanged, this, &ofApp::bpmChanged);
	ofRemoveListener(abletonLinkThread.abletonLink.numPeersChanged, this, &ofApp::numPeersChanged);
	ofRemoveListener(abletonLinkThread.abletonLink.playStateChanged, this, &ofApp::playStateChanged);

	// Kill thread
	abletonLinkThread.stopThread();

	// Close midi port
	if (USE_MIDI) {
		sendMidiOff();
		midiOut.closePort();
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	int beatCounter = -1;

	switch (key) {
		case ' ':
			toggleStartStop();
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
			// MIDI - avoid stuck notes
			sendMidiOff();

			graph = Graph((std::rand() % 20) + 5);
			graph.initLayout(myFont, myFont_adj);

			// update GUI
			dragID = 0;
			selected_bubble_id = ofToString(graph.bubbles[0].bubbleID);
			bubbleFile = graph.bubbles[0].file;
			bubbleNote = graph.bubbles[0].midi_note;
			break;

		case 'd':
			// debug beat trigger
			ofLogNotice("debug sound") << "now";
			ofNotifyEvent(abletonLinkThread.newBeatEvent, beatCounter, this);
			break;

		case 't':
			// debug direct trigger
			graph.bubbles[0].activate_sound();
			break;


		default:
			break;
	}
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// Select a bubble and update the GUI
	if (button == OF_MOUSE_BUTTON_LEFT) {
		for (int i = 0; i < graph.bubbles.size(); i++) {
			if (ofDist(x, y, graph.bubbles[i].pos.x, graph.bubbles[i].pos.y) < graph.bubbles[i].radius_animated.val()) {
				isLeftMouseDown = true;
				dragID = i;

				// update GUI
				selected_bubble_id = ofToString(graph.bubbles[i].bubbleID);
				bubbleFile = graph.bubbles[i].file;
				bubbleNote = graph.bubbles[i].midi_note;
			}
		}
	}

}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	// Drag a bubble
	if (isLeftMouseDown) {graph.bubbles[dragID].pos.set(x, y);}
}



//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	// Stop dragging a bubble
	if (button == OF_MOUSE_BUTTON_LEFT) {isLeftMouseDown = false;}
}



//--------------------------------------------------------------
/// This function is triggered by the Ableton Link thread. Whenever a new beat is triggered an event is published that triggers this function. 
void ofApp::triggerBeat(int& i){

	// Flip metronome
	metronomeCircle = !metronomeCircle;

	// Calculate the next bubble. Will set the next active bubble. 
    graph.activateNext();
 	graph.playNext(USE_MIDI); // when MIDI, dont play internal sound

	// MIDI notes on and off
	if (USE_MIDI) {
		int const activeStep = graph.getActiveStep();
		int const previousStep = graph.getPreviousStep();

		// Send off notes
		if (previousStep != -1) {
			midiOut.sendNoteOff(1, graph.bubbles[previousStep].midi_note, 64);
		}
		// Send on notes
		midiOut.sendNoteOn(1, graph.bubbles[activeStep].midi_note, 127);

		// DEBUG print out both the midi note and the frequency
		ofLogNotice("Midi note: ") << "note: " << graph.bubbles[activeStep].midi_note
			<< " freq: " << ofxMidi::mtof(graph.bubbles[activeStep].midi_note) << " Hz";
	}

}


void ofApp::toggleStartStop() {
	if (isPlaying) {
		//bpm.stop();
		ambience.pause();
		abletonLinkThread.stop_playing();
		isPlaying = false;
	}
	else {
		//bpm.start();
		ambience.play();
		abletonLinkThread.start_playing();
		isPlaying = true;
	}

	// Decative Graph
	graph.deactivateGraph();

	// Send midi off
	sendMidiOff();
}


//--------------------------------------------------------------
void ofApp::buttonGuiPressed(){
	toggleStartStop();
}


//--------------------------------------------------------------
void ofApp::bubbleNoteChanged(int& midiNote) {
	// Send midi off
	sendMidiOff();
	graph.bubbles[stoi(selected_bubble_id)].midi_note = midiNote;
}


//-----------------------------------------------f---------------
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


void ofApp::sendMidiOff() {
	if (USE_MIDI) {
		int const activeStep = graph.getActiveStep();
		if (activeStep != -1) {
			midiOut.sendNoteOff(1, graph.bubbles[graph.getActiveStep()].midi_note, 64);
		}
		int const previousStep = graph.getPreviousStep();
		if (previousStep != -1) {
			midiOut.sendNoteOff(1, graph.bubbles[previousStep].midi_note, 64);
		}
	}
}


void ofApp::bpmChanged(double& bpm) {
	ofLogNotice("bpmChanged") << bpm;
}

void ofApp::numPeersChanged(std::size_t& peers) {
	ofLogNotice("numPeersChanged") << peers;
}

void ofApp::playStateChanged(bool& state) {
	ofLogNotice("playStateChanged") << (state ? "play" : "stop");
}