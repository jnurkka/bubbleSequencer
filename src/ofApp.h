#pragma once

#include "ofMain.h"
#include "ofxBpm.h"
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

		// BPM 
		ofxBpm bpm;
		void triggerBeat();

		// GUI
		ofxPanel gui;
		ofxIntSlider int_slider;
		ofxFloatSlider f_slider_vol_ambi;
		ofxButton button;
		ofxToggle toggle_spring;
		ofxToggle hide_adj_matrix;
		void buttonGuiPressed();
		ofxLabel label_space;
		ofxLabel label_arrows;
		ofxLabel label_reset;
		ofxLabel label_full;

		// GUI for Bubble control
		ofxLabel bubbleId;
		ofxLabel bubbleFile;
		ofxIntSlider bubbleNote;
		void bubbleNoteChanged(int& midiNote);

		// MIDI
		ofxMidiOut midiOut;


	private:
		// Font
		ofTrueTypeFont myFont;
		int fontSize;

};
