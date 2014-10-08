#pragma once
#include "ofMain.h"

#include "ofxMidi.h"

class ProcessBPM_MIDI : public ofxMidiListener {

    public:
    void setup();
    void update();
    
    float getBPM();
    float getMeasurePosition();
    
    void drawAvgGraph(ofPoint pt, vector<float> values);


    ofEvent<float> everyBeat;
    ofEvent<float> firstBeat;
    ofEvent<float> midiAtom;
    //static ofEvent<float> beatBroadcast;
    
    private:
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
    vector <float> graphBPM;
    vector <float> graphSmoothBPM;
    
    float bpm;
    
    float prevBPM;
    
    int tickCounter;
    
    float prevTime;
    
    float avgBPM;
    float avgClock;
    float preClock;
    
    int beatCount;
    float measurePosition;
    
};


