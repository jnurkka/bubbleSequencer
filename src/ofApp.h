#pragma once

#include "ofMain.h"
#include "ofxBpm.h"
#include "ofxGui.h"


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
		ofxButton button;
		ofxToggle toggle_spring;
		ofxToggle hide_adj_matrix;
		void buttonGuiPressed();

		// Gui for Bubble control
		ofxLabel bubbleId;
		ofxLabel bubbleFile;
		ofxIntSlider bubbleNote;
		void bubbleNoteChanged(int& midiNote);

};
