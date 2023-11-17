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

        void init(int x, int y, int id);
        void setPos(int x, int y);
        
        int xCoord;
        int yCoord;
        int bubbleID;
        int rad;
        bool active;
        float probability;
        int graph_depth;
        int graph_width;

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
