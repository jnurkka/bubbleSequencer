//
//  eventlogImporter.hpp
//  bubbleSequencer
//
//  Created by Timothy Smith and Jaakko Nurkka on 02.12.23.
//

#pragma once
#include "ofMain.h"

class EventlogImporter {
    private:

    public:
        struct Event {
            string caseID;
            string activity;
            string timestampStr;
        };


        vector<Event> events;

        void loadCSV(const string& filePath);
        void printEventLog();

};
