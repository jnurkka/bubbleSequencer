//
//  colorManager.hpp
//  bubbleSequencer
//
//

#pragma once
#include "ofMain.h"

class ColorManager {
 
public:

    static ColorManager& getInstance(); // Singleton pattern to ensure a single instance

    ofColor getColorBackground() { return background; }
    void setColorBackground(const ofColor& c) { background = c; }

    ofColor getColorBackground2() { return background2; }
    void setColorBackground2(const ofColor& c) { background2 = c; }

    ofColor getColorEdges() { return edges; }
    void setColorEdges(const ofColor& c) { edges = c; }

    ofColor getColorInactive() { return inactive; }
    void setColorInactive(const ofColor& c) { inactive = c; }
    
    // Color 1
    ofColor getColor1() { return color1; }
    void setColor1(const ofColor& c) { color1 = c; }

    // Color 2
    ofColor getColor2() { return color2; }
    void setColor2(const ofColor& c) { color2 = c; }

    // Color 3
    ofColor getColor3() { return color3; }
    void setColor3(const ofColor& c) { color3 = c; }

    // Color 4
    ofColor getColor4() { return color4; }
    void setColor4(const ofColor& c) { color4 = c; }

    // Color 5
    ofColor getColor5() { return color5; }
    void setColor5(const ofColor& c) { color5 = c; }

    // Color 6
    ofColor getColor6() { return color6; }
    void setColor6(const ofColor& c) { color6 = c; }

    // Color 7
    ofColor getColor7() { return color7; }
    void setColor7(const ofColor& c) { color7 = c; }

    // Color 8
    ofColor getColor8() { return color8; }
    void setColor8(const ofColor& c) { color8 = c; }

    // Color 9
    ofColor getColor9() { return color9; }
    void setColor9(const ofColor& c) { color9 = c; }

    ofColor getRandomColor();


private:
    ColorManager(); // Private constructor for singleton pattern

    ofColor background;
    ofColor background2;
    ofColor edges;
    ofColor inactive;

    ofColor color1;
    ofColor color2;
    ofColor color3;
    ofColor color4;
    ofColor color5;
    ofColor color6;
    ofColor color7;
    ofColor color8;
    ofColor color9;
};

