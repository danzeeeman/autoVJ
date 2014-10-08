#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "ofxPostProcessing.h"
#include "ofxISF.h"
#include "ofxProcessFft.h"
#include "ofxBeatDetector.h"
#include "ofxPSBlend.h"
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
    
    void drawDebug();
    ofFbo mFrame;
    ofFbo mDebugFrame;
    ofImage mMask;
    ofFbo mMasker;
    ofFbo mBackColor;
    ofFbo mPing;
    ofFbo mPong;
    
    ofxPSBlend blendOne;
    ofxPSBlend blendTwo;
    
    int blendModeOne;
    int blendModeTwo;
    
    ofVideoPlayer mVideoPing;
    ofVideoPlayer mVideoPong;
    
    ofxISF::Chain mISFPing;
    ofxISF::Chain mISFPong;
    ofxISF::Chain mChainPing;
    ofxISF::Chain mChainPong;

    ofxPostGlitch mGlitch;
    ofxPostGlitch mDebugGlitch;
    ofxPostGlitch mGlitchPing;
    ofxPostGlitch mGlitchPong;
    itg::PostProcessing mPost;
    
    ofxBeatDetector* mBeat;
    
    ProcessFFT mFFt;
    float mShiftTimer;
    int mPingIndex;
    int mPongIndex;
    
    float mMeasurePos;
    ofColor mDebug;
    bool debug;
    
    float deltaShiftPong;
    float deltaShiftPing;
    ofShader mShader;
    ofRectangle mViewPort;
    float mAlpha;
    float mixPing;
    float mixPong;
    float vals[4];
    
    ofDirectory dir;
    vector<string> files;
};
