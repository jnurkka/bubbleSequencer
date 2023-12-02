//
//  ambience.cpp
//  bubbleSequencer
//
//  Created by Jaakko Nurkka on 02.12.23.
//

#include "ambience.hpp"

Ambience::Ambience(string file) {
    sample.load(file);
    sample.setVolume(0.5);
    sample.play();
    pause();
}

void Ambience::play() {
    sample.setPaused(false);
}

void Ambience::pause() {
    sample.setPaused(true);
}
