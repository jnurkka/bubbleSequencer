#pragma once

#include "ofMain.h"
#include "ofxAbletonLink.h"
#include "abletonLinkThreaded.h"
#include "ofxGui.h"
#include "ofTrueTypeFont.h"
#include "ofxMidi.h"
#include "graph.hpp"


class ofApp : public ofBaseApp{

	public:
		// OFX standard
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;
		void windowResized(int w, int h);
		float old_w;
		float old_h;


		// Graph
		Graph graph;

		// Keyboard control
		void keyPressed(int key) override;

		// Mouse control
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		bool isLeftMouseDown;
		int dragID;

		// TODO Get rid 
		//ofxBpm bpm;
		// Ablton Link
		abletonLinkThreaded abletonLinkThread;
		void bpmChanged(double& bpm);
		void numPeersChanged(std::size_t& peers);
		void playStateChanged(bool& state);
		double lastBeat;
		double currentBeat;
		bool isPlaying;

		void triggerBeat(int& i);
		void toggleStartStop();
		bool metronomeCircle;

		// GUI
		ofxPanel gui;
		ofxIntSlider int_slider;
		ofxFloatSlider f_slider_vol_ambi;
		ofxButton start_stop_button;
		ofxToggle toggle_spring;
		ofxToggle show_adj_matrix;
		ofxLabel label_space;
		ofxLabel label_arrows;
		ofxLabel label_reset;
		ofxLabel label_full;

		// GUI for Bubble control
		ofxLabel selected_bubble_id;
		ofxLabel bubbleFile;
		ofxIntSlider bubbleNote;
		void bubbleNoteChanged(int& midiNote);

		// MIDI
		ofxMidiOut midiOut;
		void sendMidiOff();
		ofxLabel label_midi_port;
		ofxLabel label_midi_name;
		ofxLabel label_midi_virtual;


		// debug
		long lastUpdateTime_update;
		long lastUpdateTime_draw;


	private:
		// Font
		ofTrueTypeFont myFont;
		ofTrueTypeFont myFont_adj;
		int fontSize;

};
