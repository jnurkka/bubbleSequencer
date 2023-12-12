//
//  ambience.hpp
//  bubbleSequencer
//
//  Created by Timothy Smith and Jaakko Nurkka on 02.12.23.
//

#pragma once
#include "ofMain.h"

class Ambience {
    private:
        ofSoundPlayer sample;
    public:
        Ambience(string file);
        void play();
        void pause();
};
