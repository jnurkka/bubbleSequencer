//
//  circle.hpp
//  creativeCoding_001
//
//
#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfColor.h"
#include "ofTrueTypeFont.h"


//#define TIME_SAMPLE
#ifdef TIME_SAMPLE
#include "ofxTimeMeasurements.h"
#endif

class Bubble {
    public:
        Bubble();
        ~Bubble();

        void init(float x, float y, int id, ofTrueTypeFont font);
        void init_sound();
        void setPos(float x, float y);

        ofVec2f pos;
        ofVec2f vel;

        
        int bubbleID;

        bool active;

        ofColor color_active_bubble;
        ofColor color_inactive_bubble;
        
        void activate_sound();
        void set_active_animations();
        void deactivate_sound();
        void set_inactive_animations();

        void draw(bool selected_bubble = false);
        void update();

        // sound
        //ofSoundStream soundStream;

        string file;
        ofSoundPlayer sample;
        ofxAnimatableFloat radius_animated;
        ofxAnimatableOfColor color_animated;

		// MIDI
        int midi_note;

private: 
    float default_radius = 30.0f;
    float active_radius = 40.0f;

    // Font
    ofTrueTypeFont myFont;

};
