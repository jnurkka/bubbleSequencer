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
		void keyPressed(int key) override;

		// BPM 
		ofxBpm bpm;
		void triggerBeat();

		// GUI
		ofxPanel gui;
		ofxIntSlider intSlider;
		ofxButton button;
		void button_gui_pressed();
};
