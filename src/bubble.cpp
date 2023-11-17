//
//  bubble.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "bubble.hpp"

Bubble::Bubble() {
	pos_x = 100;
	pos_y = 100;

	vel_x = 0;
	vel_y = 0;

	bubbleID = 0;
    rad = 10;
    active = false;
    probability = 0;
	// TODO do i need this?
	graph_depth = 0;
	graph_width = 0;
	


	color_active_bubble = ofColor::azure;
	color_inactive_bubble = ofColor::white;
}


Bubble::~Bubble() {

}


void Bubble::init(float x, float y, int id) {
	pos_x = x;
	pos_y = y;
	bubbleID = id;

	// TODO do i need this?
	graph_depth = 0;
	graph_width = 0;

	using FilenameProbabilityPair = std::tuple<std::string, float>;
	std::vector<FilenameProbabilityPair> filenameProbabilities;

	bool use_airport_samples = true;

	if (use_airport_samples)
	{
		// Copyright from music for airports
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-01.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-02.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-03.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-04.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-05.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-06.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-07.wav", 0.125f));
		filenameProbabilities.push_back(std::make_tuple("Eno-Piano-08.wav", 0.125f));
	}
	else {
		// Random shit samples
		filenameProbabilities.push_back(std::make_tuple("f.wav", 0.2f));
		filenameProbabilities.push_back(std::make_tuple("g.wav", 0.2f));
		filenameProbabilities.push_back(std::make_tuple("a.wav", 0.2f));
		filenameProbabilities.push_back(std::make_tuple("c.wav", 0.2f));
		filenameProbabilities.push_back(std::make_tuple("d.wav", 0.2f));
	}

	file = get<0>(filenameProbabilities[ofRandom(7)]); 
    probability = get<1>(filenameProbabilities[ofRandom(7)]);
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


void Bubble::setPos(float x, float y) {
	pos_x = x;
	pos_y = y;
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
	ofDrawCircle(pos_x, pos_y, radius_animated.val());

    // Plot file name
	 ofSetHexColor(0x00ff00);
	 ofDrawBitmapString(bubbleID, pos_x, pos_y);  // ofDrawBitmapString(file, pos_x, pos_y);
}
