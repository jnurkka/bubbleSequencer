#include "ofApp.h"
#include "graph.hpp"

int const GRAPH_SIZE = 10;

Graph graph(GRAPH_SIZE);

//--------------------------------------------------------------
void ofApp::setup(){
    graph.init(GRAPH_SIZE);
	myBang = false;
	x = ofGetWindowWidth() / 2;
	y = ofGetWindowHeight() / 2;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i < GRAPH_SIZE; i += 1) {
        ofSetColor(0, 255, 0);
        ofDrawCircle(graph.bubbles[i].xCoord, graph.bubbles[i].yCoord, graph.bubbles[i].rad);
    }
	if (myBang)
	{
		ofDrawCircle(x, y, 20);
	} else
	{
		ofDrawCircle(x, y, 10);
	}

}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
		if(myBang)
		{
			myBang = false;
		}
		else
		{
			myBang = true;
		}

}
