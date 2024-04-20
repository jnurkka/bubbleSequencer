#include "abletonLinkThreaded.h"

abletonLinkThreaded::abletonLinkThreaded() {
	// start the thread as soon as the
	// class is created, it won't use any CPU
	// until we send a new frame to be analyzed
	is_running = true;
	lastBeat = -1.0;
	abletonLink.setup();
	currentBeat = 0.0;
	currentTime = abletonLink.getTime();
	isPlaying = false;
	lastTime = currentTime;
	startThread();
}

abletonLinkThreaded::~abletonLinkThreaded() {
	is_running = false;
	// when the class is destroyed wait for
	// the thread to finish
	waitForThread(true);
}


void abletonLinkThreaded::setup() {
	currentBeat = abletonLink.getBeat();
	currentTime = abletonLink.getTime();
	isPlaying = abletonLink.isPlaying();
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
	
	while (is_running) {
		currentBeat = abletonLink.getBeat();
		currentTime = abletonLink.getTime();
		isPlaying = abletonLink.isPlaying();
		 
		if ((currentTime.count() - lastTime.count()) >= (60 * 1000 * 1000) / abletonLink.getBPM() && isPlaying) {
			// debug
			//ofLogNotice() << fabs(currentBeat - lastBeat);
			//ofLogNotice("Diff thread time") << currentTime.count() - lastTime.count();
			
			// fire event
			int intCounter = (int)currentBeat;
			ofNotifyEvent(newBeatEvent, intCounter, this);

			lastBeat = currentBeat;
			lastTime = currentTime;
		}
	}
}

