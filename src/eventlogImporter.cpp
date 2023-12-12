//
//  eventlogImporter.cpp
//  bubbleSequencer
//
//  Created by Timothy Smith and Jaakko Nurkka on 02.12.23.
//

#include "eventlogImporter.hpp"


void EventlogImporter::loadCSV(const string& filePath) {
    // Load CSV file
    ofBuffer buffer = ofBufferFromFile(filePath);
    string line = buffer.getNextLine();

    // Read each line from the CSV and parse the data
    while (!buffer.isLastLine()) {
        string line = buffer.getNextLine();
        stringstream ss(line);

        Event event;

        // Assuming CSV columns: Case ID, Activity, Timestamp, Resource
        getline(ss, event.caseID, ',');
        getline(ss, event.activity, ',');
        getline(ss, event.timestampStr, ',');

        // Add the parsed event to the vector
        events.push_back(event);
    }
}


void EventlogImporter::printEventLog() {
    for (const auto& event : events) {
        cout << "Case ID: " << event.caseID << ", ";
        cout << "Activity: " << event.activity << ", ";
        cout << "Timestamp: " << event.timestampStr << endl;
    }
}


vector<EventlogImporter::Event> EventlogImporter::getEventsForCaseID(const string& targetCaseID) {
    vector<Event> matchingEvents;

    for (const auto& event : events) {
        if (event.caseID == targetCaseID) {
            matchingEvents.push_back(event);
        }
    }

    return matchingEvents;
}


void EventlogImporter::printCaseEvents (vector<EventlogImporter::Event> caseEvents){
    cout << "Events for Case ID:" << endl;
    for (const auto& matchingEvent : caseEvents) {
        cout << "Case ID: " << matchingEvent.caseID << ", ";
        cout << "Activity: " << matchingEvent.activity << ", ";
        cout << "Timestamp: " << matchingEvent.timestampStr << endl;
    }
}