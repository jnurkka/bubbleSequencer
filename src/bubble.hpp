//
//  circle.hpp
//  creativeCoding_001
//
//  Created by Jaakko Nurkka on 01.11.23.
//
#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfColor.h"


//#define TIME_SAMPLE
#ifdef TIME_SAMPLE
#include "ofxTimeMeasurements.h"
#endif

class Bubble {
    public:
        Bubble();
        ~Bubble();

        void init(float x, float y, int id);
        void setPos(float x, float y);
        
        float pos_x;
        float pos_y;

        float vel_x;
        float vel_y;

        int bubbleID;

        bool active;
        float probability;

        ofColor color_active_bubble;
        ofColor color_inactive_bubble;
        
        void activate();
        void deactivate();
        void draw();
        void update();

        string file;
        ofSoundPlayer sample;
        ofxAnimatableFloat radius_animated;
        ofxAnimatableOfColor color_animated;
};
