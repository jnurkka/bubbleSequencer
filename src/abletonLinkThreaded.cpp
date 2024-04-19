#include "abletonLinkThreaded.h"

abletonLinkThreaded::abletonLinkThreaded() {
	// start the thread as soon as the
	// class is created, it won't use any CPU
	// until we send a new frame to be analyzed
	startThread();
}

abletonLinkThreaded::~abletonLinkThreaded() {
	// when the class is destroyed wait for
	// the thread to finish
	waitForThread(true);
}


void abletonLinkThreaded::setup() {
	lastBeat = -1.0;
	currentBeat = abletonLink.getBeat();
	isPlaying = abletonLink.isPlaying();
	abletonLink.setup();
	lastTime = abletonLink.getTime();
}


void abletonLinkThreaded::update() {

}


void abletonLinkThreaded::draw() {

}



void abletonLinkThreaded::start_playing() {
	abletonLink.play();
}

void abletonLinkThreaded::stop_playing() {
	abletonLink.stop();
}


void abletonLinkThreaded::threadedFunction() {
	// Ableton Link beat change
	
	while (true) {
		currentBeat = abletonLink.getBeat();
		currentTime = abletonLink.getTime();
		isPlaying = abletonLink.isPlaying();
		


		// TODO change that it compare the time passed and not the beats. exmaple: if (((double)currentTime - (double)lastTime) >= (60*1000*1000)/ abletonLink.getBPM() && isPlaying) {  
		if (fabs(currentBeat - lastBeat) >= 1.0 && isPlaying) {   // fabs(currentBeat - lastBeat) >= 1.0 && isPlaying)
			//ofLogNotice() << fabs(currentBeat - lastBeat);
			//ofLogNotice() << fabs(abletonLink.getTime() - lastTime);
			// fire event
			int intCounter = (int)currentBeat;
			ofNotifyEvent(newBeatEvent, intCounter, this);

			lastBeat = currentBeat;
		}
	}
}

