#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"

class testApp : public ofBaseApp
{
public:
    static const unsigned NUM_BOXES = 100;
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    
    // scene stuff
    ofxPostProcessing post;
    ofVideoGrabber cam;
    ofFbo fbo;
    ofLight light;
    
    // boxes
    vector<bool> enabled;
    vector<ofVec3f> posns;
    vector<ofColor> cols;
    ofVboMesh boxMesh;
};
