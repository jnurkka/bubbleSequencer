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
}

void Bubble::activate() {
    active = true;
    color = 0x008000;
    rad = 20;
	sample.play();
}

void Bubble::deactivate() {
    active = false;
    color = 0xFFFFFF;
    rad = 10;
	sample.stop();
}

void Bubble::draw()
{
	ofSetHexColor(color);
	ofDrawCircle(xCoord, yCoord, rad);

    //// Plot file name
	// ofSetHexColor(0x00ff00);
    // ofDrawBitmapString(file, xCoord, yCoord);

}
