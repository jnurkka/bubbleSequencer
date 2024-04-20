#pragma once

#ifndef OFXEVENTSADDON_H_
#define OFXEVENTSADDON_H_

#include "ofMain.h"
#include "ofxAbletonLink.h"
#include "ofEvents.h"



class abletonLinkThreaded : public ofThread {

public:
	abletonLinkThreaded();
	~abletonLinkThreaded();

	void setup();
	void update();
	void draw();

	ofxAbletonLink abletonLink;
	void start_playing();
	void stop_playing();

	double lastBeat;
	double currentBeat;
	std::chrono::microseconds currentTime;
	std::chrono::microseconds lastTime;
	bool isPlaying;

	ofEvent<int> newBeatEvent;

	bool is_running;
private:
	void threadedFunction();
};


#endif