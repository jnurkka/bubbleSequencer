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

		// Keyboard control
		void keyPressed(int key) override;

		// Mouse control
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		bool isDragging;
		int dragID;

		// BPM 
		ofxBpm bpm;
		void triggerBeat();

		// GUI
		ofxPanel gui;
		ofxIntSlider int_slider;
		ofxButton button;
		ofxToggle toggle_spring;
		void button_gui_pressed();

		// Gui for Bubble control
		ofxLabel bubbleId;
		ofxLabel bubbleFile;
		ofxIntSlider bubbleNote;
		void bubble_note_changed(int& midiNote);

};
