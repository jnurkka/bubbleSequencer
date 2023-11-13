//
//  bubble.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "bubble.hpp"

Bubble::Bubble() {
    xCoord = 100;
    yCoord = 100;
    rad = 10;
    active = false;
    color = 0xFFFFFF;
    probability = 0;
}

void Bubble::init(int x, int y, int radius, string filename, float p) {
    xCoord = x;
    yCoord = y;
    rad = radius;
    file = filename;
    probability = p;
    sample.load(file);
    sample.setVolume(0.5);
    sample.setMultiPlay(true);

	// Animations
	radius_animated.reset(10);

}

void Bubble::activate() {
    active = true;
    color = 0x008000;
    rad = 20;
	sample.play();

	// Animations
	radius_animated.reset(10);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1);
	radius_animated.animateTo(20);
}

void Bubble::deactivate() {
    active = false;
    color = 0xFFFFFF;
    rad = 10;
	//sample.stop();

	// Animations
	radius_animated.reset(20);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1);
	radius_animated.animateTo(10);
}



void Bubble::update()
{
	//app timebase, to send to all animatable objects
	float dt = 1.0f / 60.0f;

    radius_animated.update(dt);
}

void Bubble::draw()
{
	ofSetHexColor(color);
	ofDrawCircle(xCoord, yCoord, radius_animated.val());

    //// Plot file name
	// ofSetHexColor(0x00ff00);
    // ofDrawBitmapString(file, xCoord, yCoord);

}
