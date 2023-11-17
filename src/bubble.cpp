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
    probability = 0;


	color_active_bubble = ofColor::azure;
	color_inactive_bubble = ofColor::white;
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

	//// Animations
	// radius
	radius_animated.reset(10);
	// color
	color_active_bubble.setHex(0xFF6F3D);  //TODO: use this color for edges between nodes 0xF3ECDB
	color_inactive_bubble.setHex(0x7F886A );
	color_animated.setColor(color_inactive_bubble);

}

void Bubble::activate() {
    active = true;
    rad = 20;
	sample.play();

	// Animations
	radius_animated.reset(10);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1);
	radius_animated.animateTo(30);

	color_animated.setColor(color_inactive_bubble);
	color_animated.setDuration(1);
	color_animated.setRepeatType(PLAY_ONCE);
	color_animated.setCurve(LINEAR);
	color_animated.animateTo(color_active_bubble);
}

void Bubble::deactivate() {
    active = false;
    rad = 10;
	//sample.stop();

	// Animations
	radius_animated.reset(30);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1);
	radius_animated.animateTo(10);

	color_animated.setColor(color_active_bubble);
	color_animated.setDuration(1);
	color_animated.setRepeatType(PLAY_ONCE);
	color_animated.setCurve(LINEAR);
	color_animated.animateTo(color_inactive_bubble);
}



void Bubble::update()
{
	//app timebase, to send to all animatable objects
	float dt = 1.0f / 60.0f;

    radius_animated.update(dt);
	color_animated.update(dt);
}

void Bubble::draw()
{
	// Set color and draw bubble
	color_animated.applyCurrentColor();
	ofDrawCircle(xCoord, yCoord, radius_animated.val());

    //// Plot file name
	// ofSetHexColor(0x00ff00);
    // ofDrawBitmapString(file, xCoord, yCoord);

}
