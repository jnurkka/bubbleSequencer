//
//  colorManager.cpp
//  bubbleSequencer
//
//

#include "colorManager.hpp"

ColorManager& ColorManager::getInstance() {
    static ColorManager instance; // Guaranteed to be destroyed, instantiated on first use
    return instance;
}

ColorManager::ColorManager() {

    setColorBackground(ofColor::fromHex(0x050E1F)); // darkblue
    setColorBackground2(ofColor::fromHex(0x0B2349)); // little lighterdarkblue
    setColorEdges(ofColor::fromHex(0xFFFFFF));  // white
    setColorInactive(ofColor::fromHex(0x666666));  // grey

    setColor1(ofColor::fromHex(0x1274FA)); // blue
    setColor2(ofColor::fromHex(0x00FEF9)); // cyan
    setColor3(ofColor::fromHex(0x0AFD70)); // Green
    setColor4(ofColor::fromHex(0x05FFBA)); // turquoise
    setColor5(ofColor::fromHex(0x8051FE)); // purple
    setColor6(ofColor::fromHex(0xFE6AFD)); // pink
    setColor7(ofColor::fromHex(0xF41025)); // red
    setColor8(ofColor::fromHex(0xFEFE00)); // yellow
    setColor9(ofColor::fromHex(0xFF560D)); // orange
}


ofColor ColorManager::getRandomColor() {
    // Create an array of colors to choose from
    std::vector<ofColor> colors = { color1, color2, color3, color4, color5, color6, color7, color8, color9 };

    // Randomly choose a color from the array
    int randomIndex = ofRandom(0, colors.size());
    return colors[randomIndex];
}

