#pragma once

#include "ofMain.h"
#include "ofxISF.h"
#include "ofxProcessFft.h"
#include "ofxBeatDetector.h"
#include "ofxSyphon.h"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    
    void allocateFbos();
    void initShaderChains();
    
    ofFbo mFrame;
    ofImage mMask;
    ofFbo mPing;
    ofFbo mPong;

    int blendModeOne;
    int blendModeTwo;
    
    ofxISF::Chain mISFPing;
    ofxISF::Chain mISFPong;
    ofxISF::Chain mChainPing;
    ofxISF::Chain mChainPong;
    ofxISF::Chain mAbc;

    
    ofxBeatDetector* mBeat;
    
    ProcessFFT mFFt;
    float mShiftTimer;
    int mPingIndex;
    int mPongIndex;
    
    float mMeasurePos;
    ofColor mDebug;
    bool debug;
    
    bool triggered;
    int tcount;
    
    float deltaShiftPong;
    float deltaShiftPing;
    ofShader mShader;
    ofRectangle mViewPort;
    float mAlpha;
    float mixPing;
    float mixPong;
    float vals[4];
    bool bFullscreen;
    float previousBlend1;
    float previousBlend2;
    
    ofDirectory dir;
    vector<string> files;
    
    ofShader shader;
    
    ofCamera cam;
    float stage;
    float abcAlpha;
    
    bool fade;
    ofVboMesh mesh;
    
    ofBoxPrimitive plane;
    
    
    ofQuaternion left;
    ofQuaternion right;
    ofQuaternion up;
    ofQuaternion down;
    bool drawFPS;
    
    bool kick, hat, snare, low, mid, high, midhigh;
    float lastKick, lastHat, lastSnare, lastLow, lastMid, lastHigh, lastMidhigh;
    float kickDiff, hatDiff, snareDiff, lowDiff, midDiff, highDiff, midhighDiff;
    
    ofxSyphonServer server;
    
    bool fooScreenCap;
};
