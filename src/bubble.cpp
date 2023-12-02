//
//  bubble.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 01.11.23.
//

#include "bubble.hpp"

Bubble::Bubble() {
	;
	pos = ofVec2f(100, 100);
	vel = ofVec2f(0, 0);

	bubbleID = 0;
	active = false;
	probability = 0;

	color_active_bubble = ofColor::azure;
	color_inactive_bubble = ofColor::white;

	midi_note = rand() % 128;
}


Bubble::~Bubble() {

}


void Bubble::init(float x, float y, int id, ofTrueTypeFont font) {
	pos = ofVec2f(x, y);
	
	bubbleID = id;

	myFont = font;

	using FilenameProbabilityPair = std::tuple<std::string, float>;
	std::vector<FilenameProbabilityPair> filenameProbabilities;

	bool use_airport_samples = true;

	if (use_airport_samples)
	{
		// Copyright from music for airports
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-01.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-02.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-03.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-04.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-05.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-06.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-07.wav", 0.125f));
		filenameProbabilities.emplace_back(std::make_tuple("Eno-Piano-08.wav", 0.125f));
	}
	else {
		// Random shit samples
		filenameProbabilities.emplace_back(std::make_tuple("f.wav", 0.2f));
		filenameProbabilities.emplace_back(std::make_tuple("g.wav", 0.2f));
		filenameProbabilities.emplace_back(std::make_tuple("a.wav", 0.2f));
		filenameProbabilities.emplace_back(std::make_tuple("c.wav", 0.2f));
		filenameProbabilities.emplace_back(std::make_tuple("d.wav", 0.2f));
	}

	file = get<0>(filenameProbabilities[ofRandom(7)]); 
    probability = get<1>(filenameProbabilities[ofRandom(7)]);
    sample.load(file);
    sample.setVolume(0.5);
    sample.setMultiPlay(true);

	//// Animations
	// radius
	radius_animated.reset(default_radius);
	// color
	color_active_bubble.setHex(0xFF6F3D);  // TODO: use this color for edges between nodes 0xF3ECDB
	color_inactive_bubble.setHex(0x7F886A );
	color_animated.setColor(color_inactive_bubble);

}


void Bubble::activate() {
    active = true;
	sample.play();

	// Animations
	radius_animated.reset(default_radius);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1);
	radius_animated.animateTo(active_radius);

	color_animated.setColor(color_inactive_bubble);
	color_animated.setDuration(1);
	color_animated.setRepeatType(PLAY_ONCE);
	color_animated.setCurve(LINEAR);
	color_animated.animateTo(color_active_bubble);
}


void Bubble::deactivate() {
    active = false;

	// Animations
	radius_animated.reset(active_radius);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1);
	radius_animated.animateTo(default_radius);

	color_animated.setColor(color_active_bubble);
	color_animated.setDuration(1);
	color_animated.setRepeatType(PLAY_ONCE);
	color_animated.setCurve(LINEAR);
	color_animated.animateTo(color_inactive_bubble);
}


void Bubble::setPos(const float x, const float y) {
	pos.x = x;
	pos.y = y;
}


void Bubble::update()
{
	//app timebase, to send to all Animatable objects
	float constexpr dt = 1.0f / 60.0f;

    radius_animated.update(dt);
	color_animated.update(dt);
}


void Bubble::draw(bool selected)
{
	// Set color and draw bubble
	color_animated.applyCurrentColor();
	ofDrawCircle(pos.x, pos.y, radius_animated.val());
	
	// Draw outline if selected by GUI
	if (selected)
	{
		ofPushMatrix();
		// Draw an outlined circle around it
		ofSetColor(255);  
		ofNoFill();  // Switch to drawing an outline
		ofDrawEllipse(pos, radius_animated.val() * 2, radius_animated.val() * 2);
		ofFill();  // Switch back to filling shapes
		ofPopMatrix();
	}

	// Plot bubble ID. Estimate bounding box based on character count
	ofSetHexColor(0xF3ECDB);
	std::string idString = std::to_string(bubbleID);
	ofRectangle boundingBox = ofRectangle(0, 0, idString.length() * 8, 12); 
	ofPoint textPosition(pos.x - boundingBox.width / 2, pos.y + boundingBox.height / 2);
	myFont.drawString(idString, textPosition.x, textPosition.y);

}