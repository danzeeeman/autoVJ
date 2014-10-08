#pragma once

#include "ofMain.h"
#include "ofxPSBlend.h"

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    ofxPSBlend psBlend;
    ofImage base;
    ofImage target0;
    ofImage target1;
    ofImage target2;
    ofImage target3;
    ofImage target4;
    ofImage target5;
    ofImage target6;
    ofImage target7;
    int blendMode;

};
