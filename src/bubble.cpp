//
//  bubble.cpp
//  bubbleSequencer
//
//

#include "bubble.hpp"
#include <filesystem>
#include "ColorManager.hpp"


namespace fs = std::filesystem;


Bubble::Bubble() {
	;
	pos = ofVec2f(100, 100);
	vel = ofVec2f(0, 0);

	bubbleID = 0;
	active = false;


	color_active_bubble = ofColor::azure;
	color_inactive_bubble = ofColor::white;

	midi_note = ofRandom(40, 121);

}


Bubble::~Bubble() {

}


void Bubble::init(float x, float y, int id, ofTrueTypeFont font) {
	
	// Meta data
	pos = ofVec2f(x, y);
	bubbleID = id;
	myFont = font;

	// Sounds
	init_sound();


	//// Animations
	// radius
	radius_animated.reset(default_radius);
	// color
	color_active_bubble.set(ColorManager::getInstance().getRandomColor());
	color_inactive_bubble.set(ColorManager::getInstance().getColorInactive());
	color_animated.setColor(color_inactive_bubble);
}


void Bubble::init_sound() {
	/*/ Init sound
	ofSoundStreamSettings settings;
	settings.setOutListener(this);
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.sampleRate = 44100;
	settings.bufferSize = 32;

	soundStream.setup(settings); */
	
	// choose random sample
	string path = "bells";
	ofDirectory dir(path);
	dir.allowExt("mp3");
	dir.listDir();

	// Check if there are any files in the directory
	if (dir.size() == 0) {
		ofLogError("ofApp::setup") << "No files found in the folder" << endl;
		return;
	}

	// Get the path of the randomly chosen file
	int randomIndex = ofRandom(0, dir.size());
	string randomFile = dir.getPath(randomIndex);

	// init sample player
	sample.load(randomFile);
	sample.setVolume(0.5);
	sample.setMultiPlay(true);

	// pre-load sound for less delay 
	sample.play();
	sample.stop();
}


void Bubble::activate_sound() {
	active = true;
	sample.play();
}


void Bubble::activate_ui() {
	// Animations
	radius_animated.reset(default_radius);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(0.5f);
	radius_animated.animateTo(active_radius);

	color_animated.setColor(color_inactive_bubble);
	color_animated.setDuration(0.2);
	color_animated.setRepeatType(PLAY_ONCE);
	color_animated.setCurve(LINEAR);
	color_animated.animateTo(color_active_bubble);
}


void Bubble::deactivate_sound() {
	active = false;
}


void Bubble::deactivate_ui() {
	// Animations
	radius_animated.reset(active_radius);
	radius_animated.setCurve(EASE_OUT_BACK);
	radius_animated.setRepeatType(PLAY_ONCE);
	radius_animated.setDuration(1.0);
	radius_animated.animateTo(default_radius);

	color_animated.setColor(color_active_bubble);
	color_animated.setDuration(1.0);
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
	float dt = 1.0f / ofGetFrameRate();

    radius_animated.update(dt);
	color_animated.update(dt);
}


void Bubble::draw(bool selected_bubble)
{
	// Set color and draw bubble
	color_animated.applyCurrentColor();
	ofDrawCircle(pos.x, pos.y, radius_animated.val());
	
	// Draw outline if selected by GUI
	if (selected_bubble)
	{
		ofPushMatrix();
		// Draw an outlined circle around it
		ofSetColor(ColorManager::getInstance().getColorEdges());
		ofNoFill();  // Switch to drawing an outline
		ofDrawEllipse(pos, radius_animated.val() * 2, radius_animated.val() * 2);
		ofFill();  // Switch back to filling shapes
		ofPopMatrix();
	}

	// Plot bubble ID. Estimate bounding box based on character count
	ofSetColor(ColorManager::getInstance().getColorBackground());
	std::string idString = std::to_string(bubbleID);
	ofRectangle boundingBox = ofRectangle(0, 0, idString.length() * 8, 12); 
	ofPoint textPosition(pos.x - boundingBox.width / 2, pos.y + boundingBox.height / 2);
	myFont.drawString(idString, textPosition.x, textPosition.y);

}
