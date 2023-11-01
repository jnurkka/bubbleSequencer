#include "ofApp.h"
#include "graph.hpp"

int const GRAPH_SIZE = 10;

Graph graph(GRAPH_SIZE);

//--------------------------------------------------------------
void ofApp::setup(){
    graph.init(GRAPH_SIZE);



	// bang visualise
	myBang = false;
	x = ofGetWindowWidth() / 2;
	y = ofGetWindowHeight() / 2;

	// BPM
	bpm.setBeatPerBar(4);
	bpm.setBpm(120);

	ofAddListener(bpm.beatEvent, this, &ofApp::triggerBeat);

	bpm.stop();

	// GUI
	gui.setup();

	button.addListener(this, &ofApp::buttonGUIpressed);

	gui.add(intSlider.setup("BPM Slider", 120, ofxBpm::OFX_BPM_MIN, ofxBpm::OFX_BPM_MAX));
	gui.add(button.setup("Start/stop"));

}

//--------------------------------------------------------------
void ofApp::update(){
	bpm.setBpm(intSlider);
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i < GRAPH_SIZE; i += 1) {
        ofSetHexColor(graph.bubbles[i].color);
        ofDrawCircle(graph.bubbles[i].xCoord, graph.bubbles[i].yCoord, graph.bubbles[i].rad);
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::triggerBeat()
{
    graph.activateNext();
}

void ofApp::buttonGUIpressed()
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
