//
//  circle.hpp
//  creativeCoding_001
//
//  Created by Jaakko Nurkka on 01.11.23.
//
#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"


//#define TIME_SAMPLE
#ifdef TIME_SAMPLE
#include "ofxTimeMeasurements.h"
#endif

class Bubble {
    public:
        int xCoord;
        int yCoord;
        int rad;
        bool active;
        float probability;
        string file;
        int color;
        Bubble();
        void init(int x, int y, int radius, string filename, float p);
        void activate();
        void deactivate();
        void draw();
        void update();

        ofSoundPlayer sample;
        ofxAnimatableFloat radius_animated;

};
