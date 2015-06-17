#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetWindowPosition(ofGetScreenWidth()+ofGetScreenWidth()/2, 0);
    //ofSetFullscreen(false);
    //ofSetDataPathRoot("../Resources/data/");
    
    
    mFFt.setup();
    mFFt.setNormalize(true);
    mFFt.setExponent(1.75);
    mFFt.setFFTpercentage(0.14);
    mFFt.setNumFFTBins(256);
    
    mBeat = new ofxBeatDetector();
    mBeat->enableBeatDetect();
    mBeat->setBeatValue(1.25);
    mShiftTimer = ofGetElapsedTimef();
    
    mixPing = 125;
    
    
    blendModeOne = 0;
    blendModeTwo = 0;

    stage = 0.0;
    mViewPort = ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    allocateFbos();
    
    
    debug = false;
    
    server.setName("autoVJ");
    //ofSetWindowPosition(0, ofGetScreenHeight()-ofGetWindowHeight());
    
}
void ofApp::allocateFbos(){
    mFrame.allocate(512, 512, GL_RGBA);
    mPing.allocate(512, 512, GL_RGBA);
    mPong.allocate(512, 512, GL_RGBA);
    
    mFrame.begin();
    ofClear(0, 0, 0, 0);
    mFrame.end();
    
    mPing.begin();
    ofClear(0, 0, 0, 0);
    mPing.end();
    
    mPong.begin();
    ofClear(0, 0, 0, 0);
    mPong.end();
    
    initShaderChains();
}
void ofApp::initShaderChains(){
    
    
    mChainPing.setup(512, 512);
    mChainPong.setup(512, 512);
    mISFPing.setup(512, 512);
    mISFPong.setup(512, 512);
    mAbc.setup(512, 512);
    
    dir.allowExt("fs");
    dir.allowExt("vs");
    dir.listDir(ofToDataPath("isf", true));
    dir.sort();
    int count = 0;
    for(int i = 0; i < dir.size(); i++){
        // Multi Pass Gaussian and Layer Masks are currently broken in OF
        if(!ofIsStringInString(dir.getPath(i), "Multi Pass Gaussian") && !ofIsStringInString(dir.getPath(i), "Layer Mask") && !ofIsStringInString(dir.getPath(i), "3d Rotate") && !ofIsStringInString(dir.getPath(i), "Auto Levels") && !ofIsStringInString(dir.getPath(i), ".qtz.") && !ofIsStringInString(dir.getPath(i), "G") && !ofIsStringInString(dir.getPath(i), "B") && !ofIsStringInString(dir.getPath(i), "vv") && !ofIsStringInString(dir.getPath(i), "Pass")){
            cout<<dir.getPath(i)<<endl;
            if(mISFPing.load(dir.getPath(i))){
                mISFPing.setEnable(count, false);
                mISFPong.load((dir.getPath(i)));
                mISFPong.setEnable(count, false);
                count++;
                files.push_back(dir.getPath(i));
            }
        }
    }
    
    
    dir.allowExt("fs");
    dir.allowExt("vs");
    dir.listDir(ofToDataPath("chains/one", true));
    dir.sort();
    count = 0;
    for(int i = 0; i < dir.size(); i++){
        // Multi Pass Gaussian and Layer Masks are currently broken in OF
        if(!ofIsStringInString(dir.getPath(i), "Multi Pass Gaussian") && !ofIsStringInString(dir.getPath(i), "Layer Mask") && !ofIsStringInString(dir.getPath(i), "3d Rotate") && !ofIsStringInString(dir.getPath(i), "Auto Levels") && !ofIsStringInString(dir.getPath(i), ".qtz.") && !ofIsStringInString(dir.getPath(i), "G") && !ofIsStringInString(dir.getPath(i), "B") && !ofIsStringInString(dir.getPath(i), "vv") && !ofIsStringInString(dir.getPath(i), "Pass")){
            cout<<dir.getPath(i)<<endl;
            if(mChainPing.load(dir.getPath(i))){
                mChainPing.setEnable(count, true);
                count++;
            }
        }
    }
    
    dir.allowExt("fs");
    dir.allowExt("vs");
    dir.listDir(ofToDataPath("chains/two", true));
    dir.sort();
    count = 0;
    for(int i = 0; i < dir.size(); i++){
        // Multi Pass Gaussian and Layer Masks are currently broken in OF
        if(!ofIsStringInString(dir.getPath(i), "Multi Pass Gaussian") && !ofIsStringInString(dir.getPath(i), "Layer Mask") && !ofIsStringInString(dir.getPath(i), "3d Rotate") && !ofIsStringInString(dir.getPath(i), "Auto Levels") && !ofIsStringInString(dir.getPath(i), ".qtz.") && !ofIsStringInString(dir.getPath(i), "G") && !ofIsStringInString(dir.getPath(i), "B") && !ofIsStringInString(dir.getPath(i), "vv") && !ofIsStringInString(dir.getPath(i), "Pass")){
            cout<<dir.getPath(i)<<endl;
            if(mChainPong.load(dir.getPath(i))){
                mChainPong.setEnable(count, true);
                count++;
            }
        }
    }
    
    dir.allowExt("fs");
    dir.allowExt("vs");
    dir.listDir(ofToDataPath("chains/abc", true));
    dir.sort();
    count = 0;
    for(int i = 0; i < dir.size(); i++){
        // Multi Pass Gaussian and Layer Masks are currently broken in OF
        if(!ofIsStringInString(dir.getPath(i), "Multi Pass Gaussian") && !ofIsStringInString(dir.getPath(i), "Layer Mask") && !ofIsStringInString(dir.getPath(i), "3d Rotate") && !ofIsStringInString(dir.getPath(i), "Auto Levels") && !ofIsStringInString(dir.getPath(i), ".qtz.") && !ofIsStringInString(dir.getPath(i), "G") && !ofIsStringInString(dir.getPath(i), "B") && !ofIsStringInString(dir.getPath(i), "vv") && !ofIsStringInString(dir.getPath(i), "Pass")){
            cout<<dir.getPath(i)<<endl;
            if(mAbc.load(dir.getPath(i))){
                mAbc.setEnable(count, true);
                count++;
            }
        }
    }
    
    
    mPongIndex = 0;
    mPingIndex = 0;
    mISFPing.setEnable(mPingIndex, true);
    mISFPong.setEnable(mPongIndex, true);
    
    mISFPing.setImage(mPong.getTexture());
    mISFPong.setImage(mPing.getTexture());
    
    mChainPong.setImage(mPong.getTexture());
    mChainPing.setImage(mPing.getTexture());
    
    
    mAbc.setImage(mFrame.getTexture());

    
    mShader.load(ofToDataPath("displacement/shadersGL2/shader.vert"), ofToDataPath("displacement/shadersGL2/shader.frag"));
    plane.set(12000,12000,12000, 150, 150, 150);
    
    
    
    plane.setPosition(0, 0, 0);
    
    plane.mapTexCoordsFromTexture(mFrame.getTexture());
    
    mesh = plane.getMesh();
    
}
//--------------------------------------------------------------
void ofApp::update(){
    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    mFFt.update();
    mBeat->update(&mFFt.getSpectrum()[0]);
    

    vals[0] = (ofLerp(vals[0], mFFt.getLowVal(), 0.1225));
    vals[1] = (ofLerp(vals[1], mFFt.getMidVal(), 0.1225));
    vals[2] = (ofLerp(vals[2], mFFt.getHighVal(), 0.09225));
    vals[3] = (ofLerp(vals[3], mFFt.getSuperLowVal(), 0.09225));
    
    
    
    if(mBeat->isHat()){
        hat=true;
        lastHat = ofGetElapsedTimef();
    }
    if(mBeat->isKick()){
        kick=true;
        lastKick = ofGetElapsedTimef();
    }
    if(mBeat->isSnare()){
        snare=true;
        lastSnare = ofGetElapsedTimef();
    }
    if(mBeat->isLow()){
        low = true;
        lastLow = ofGetElapsedTimef();
    }
    if(mBeat->isHigh()){
        high= true;
        lastHigh = ofGetElapsedTimef();
    }
    if(mBeat->isMid()){
        mid = true;
        lastMid = ofGetElapsedTimef();
    }
    if(mBeat->isMidHigh()){
        midhigh = true;
        lastMidhigh = ofGetElapsedTimef();
        
    }
    midhighDiff = ofGetElapsedTimef()-lastMidhigh;
    midDiff = ofGetElapsedTimef()-lastMid;
    highDiff = ofGetElapsedTimef()-lastHigh;
    lowDiff = ofGetElapsedTimef()-lastLow;
    snareDiff = ofGetElapsedTimef()-lastSnare;
    kickDiff = ofGetElapsedTimef()-lastKick;
    hatDiff = ofGetElapsedTimef()-lastHat;
    
    
    if(lowDiff > 1.5){
        low = false;
    }
    if(highDiff > 1.5){
        high = false;
    }
    if(snareDiff > 1.5){
        snare = false;
    }
    if(midhighDiff > 1.5){
        midhigh = false;
    }
    if(hatDiff > 1.5){
        hat = false;
    }
    if(kickDiff > 1.5){
        kick = false;
    }
    if(midDiff > 1.5){
        mid = false;
    }
    
    left.makeRotate(360*sin(ofGetElapsedTimef()*0.066), 1, 0, 0);
    right.makeRotate(360*cos(ofGetElapsedTimef()*0.066), 1, 0, 0);
    up.makeRotate(360*cos(ofGetElapsedTimef()*0.066), 0, 1, 0);
    down.makeRotate(360*sin(ofGetElapsedTimef()*0.066), 0, 1, 0);
    
    ofQuaternion q;
    q = cam.getOrientationQuat();
    if(hat || high){
        q.slerp(0.15*vals[0], q, left*down);
    }else if(mid || low || midhigh){
        q.slerp(0.15*vals[1], q, up);
    }else if(kick || snare){
        q.slerp(0.15*vals[3], q, down*right);
    }

    
    cam.setOrientation(q);
    
//    vals[0]/=1.0;
//    vals[1]/=1.0;
//    vals[2]/=1.0;
//    vals[3]/=2.0;
    //
    
    

    
    cam.setPosition(ofVec3f(0, 0, 0));
    
    
    
    
    mChainPong.getShader("4Kaleidoscope Tile")->setUniform<float>("sides", ofMap(vals[1], 0, 1, 6.0, 32.0, true));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<float>("sides", ofMap(vals[1], 0, 1, 6.0, 32.0, true));
    mChainPong.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide1", ofVec2f(ofMap(vals[2], 0, 1, -1.0, 1.0, true), ofMap(vals[2], 0, 1, -1.0, 1.0, true)));
    mChainPong.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide2", ofVec2f(ofMap(vals[1], 0, 1, -1.0, 1.0, true), ofMap(vals[1], 0, 1, -1.0, 1.0, true)));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<float>("sides", ofMap(mFFt.getMidVal(), 0, 1, 6.0, 32.0, true));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide1", ofVec2f(ofMap(vals[3], 0, 1, -1.0, 1.0, true), ofMap(vals[3], 0, 1, -1.0, 1.0, true)));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide2", ofVec2f(ofMap(vals[1], 0, 1, -1.0, 1.0, true), ofMap(vals[1], 0, 1, -1.0, 1.0, true)));
    
    mChainPing.getShader("3QuadTile")->setUniform<float>("size", ofMap(vals[2], 0, 1, 0.0, 2.0, true));
    mChainPong.getShader("3QuadTile")->setUniform<float>("size", ofMap(vals[1], 0, 1, 0.0, 2.0, true));
    mChainPing.getShader("3QuadTile")->setUniform<float>("rotation", ofMap(vals[2], 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("3QuadTile")->setUniform<ofVec2f>("slide1", ofVec2f(ofMap(vals[2], 0, 1, -1.0, 1.0, true), ofMap(vals[2], 0, 1, -1.0, 1.0, true)));
    mChainPing.getShader("3QuadTile")->setUniform<ofVec2f>("slide2", ofVec2f(ofMap(vals[1], 0, 1, -1.0, 1.0, true), ofMap(vals[1], 0, 1, -1.0, 1.0, true)));
    
    
    mChainPong.getShader("3QuadTile")->setUniform<float>("rotation", ofMap(vals[0], 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("3QuadTile")->setUniform<ofVec2f>("slide1", ofVec2f(ofMap(vals[1], 0, 1, -1.0, 1.0, true), ofMap(vals[1], 0, 1, -1.0, 1.0, true)));
    mChainPong.getShader("3QuadTile")->setUniform<ofVec2f>("slide2", ofVec2f(ofMap(vals[2], 0, 1, -1.0, 1.0, true), ofMap(vals[2], 0, 1, -1.0, 1.0, true)));
    
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("sides", ofMap(vals[0], 0, 1, 6.0, 32.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("slidey",ofMap(vals[1], 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("angle",ofMap(vals[2], 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("slidex", ofMap(vals[3], 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<ofVec2f>("center", ofVec2f(mChainPing.getWidth()/2, mChainPing.getHeight()/2));
    
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("sides", ofMap(vals[0], 0, 1, 6.0, 32.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("angle",ofMap(vals[1], 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("slidey",ofMap(vals[2], 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("slidex", ofMap(vals[3], 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<ofVec2f>("center", ofVec2f(mChainPing.getWidth()/2,  mChainPing.getHeight()/2));
    
    
    mChainPong.getShader("9Color Controls")->setUniform<float>("bright", -0.1);
    mChainPong.getShader("9Color Controls")->setUniform<float>("contrast",ofMap(vals[1], 0, 1, 3.5, 4.0, true));
    mChainPong.getShader("9Color Controls")->setUniform<float>("hue",ofMap(vals[2], 0, 1, -1.0, 1.0, true));
    mChainPong.getShader("9Color Controls")->setUniform<float>("saturation", ofMap(vals[3], 0, 1, 4.0, 2.0, true));
    
    
    mChainPing.getShader("9Color Controls")->setUniform<float>("bright", -0.1);
    mChainPing.getShader("9Color Controls")->setUniform<float>("contrast",ofMap(vals[1], 0, 1, 3.5,4.0, true));
    mChainPing.getShader("9Color Controls")->setUniform<float>("hue",ofMap(vals[2], 0, 1, -1.0, 1.0, true));
    mChainPing.getShader("9Color Controls")->setUniform<float>("saturation", ofMap(vals[3], 0, 1, 4.0, 2.0, true));
    
    mAbc.getShader("9Color Controls")->setUniform<float>("bright", -0.1);
    mAbc.getShader("9Color Controls")->setUniform<float>("contrast",4.0);
    mAbc.getShader("9Color Controls")->setUniform<float>("hue",ofMap(vals[1], 0, 1, -1.0, 1.0, true));
    mAbc.getShader("9Color Controls")->setUniform<float>("saturation", ofMap(vals[3], 0, 1, 4.0, 2.0, true));
    

    ofEnableAlphaBlending();
    mPing.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255);
    mISFPong.draw(0, 0, 512, 512);
    ofDisableAlphaBlending();
    ofPopStyle();
    mPing.end();
    
    
    
    mPong.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255);
    mISFPing.draw(0, 0, 512, 512);
    ofDisableAlphaBlending();
    ofPopStyle();
    mPong.end();
    
    
    
    if(!fade && abcAlpha < 1.0){
        abcAlpha = ofLerp(abcAlpha, 1.0, 0.1);
    }else if(fade && abcAlpha > 0.0){
        abcAlpha = ofLerp(abcAlpha, 0.0, 0.1);
    }
    
    if ((abs(mFFt.getDelta())>(mFFt.getSmoothedUnScaledLoudestValue()*0.22)) && mFFt.getUnScaledLoudestValue() > 0.1 && (ofGetElapsedTimef() - deltaShiftPong)>6.0 ) {
        deltaShiftPong = ofGetElapsedTimef();
        mISFPing.setEnable(mPingIndex, false);
        mPingIndex--;
        if(mPingIndex < 0){
            mPingIndex = mISFPing.size()-1;;
        }
        mISFPing.setEnable(mPingIndex, true);
    }else if((abs(mFFt.getDelta())>(mFFt.getSmoothedUnScaledLoudestValue()*0.33)) && mFFt.getUnScaledLoudestValue() > 0.1 && (ofGetElapsedTimef() - deltaShiftPing)>6.0){
        deltaShiftPing = ofGetElapsedTimef();
        mISFPong.setEnable(mPongIndex, false);
        mPongIndex++;
        if(mPongIndex >= mISFPong.size()){
            mPongIndex =0;
        }
        mISFPong.setEnable(mPongIndex, true);
    }
    
    
    previousBlend1 = ofLerp(previousBlend1,ofMap(vals[0]+vals[2], 0, 2, 175, 250,true), 0.5);
    
    previousBlend2 = ofLerp(previousBlend2,ofMap(vals[1]+vals[3], 0, 2, 175, 250,true), 0.5);
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    mFrame.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(255,  previousBlend1);
    mChainPing.draw(0, 0);
    ofSetColor(255, previousBlend2);
    mChainPong.draw(0, 0);
    mFrame.end();
    
    
    
    mAbc.update();
    mISFPing.update();
    mISFPong.update();
    mChainPing.update();
    mChainPong.update();
    
    
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    
    ofSetLineWidth(1.0);
    ofSetColor(255, 255, 255);
    cam.begin(mViewPort);
    mAbc.getTexture().bind();
    mShader.begin();
    plane.drawWireframe();
    mShader.end();
    mAbc.getTexture().unbind();
    cam.end();
    
    server.publishScreen();
    
    
    ofSetColor(255, 255);
    if(debug){
        mPong.draw(50, 50, 512/2, 512/2);
        mPing.draw(50+512/2, 50, 512/2, 512/2);
        mFrame.draw(50+2*512/2, 50, 512/2, 512/2);
        mChainPong.draw(50, 50+512/2, 512/2, 512/2);
        mChainPing.draw(50+512/2, 50+512/2, 512/2, 512/2);
        mAbc.draw(50+512/2*2, 50+512/2, 512/2, 512/2);
        
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("DEBUG MODE PRESS D TO GO LIVE", ofGetWidth()-300, ofGetHeight()-25);
        
        mBeat->drawSmoothFFT();
    }
    ofDisableAlphaBlending();
    
    if(drawFPS){
        ofSetColor(255, 0, 255);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth()-300, ofGetHeight()-25);
    }
    ofSaveScreen(ofGetTimestampString()+".png");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'd'){
        debug = !debug;
        //df  = false;
        //ofSetFullscreen(!debug);
    }
    if(key == '1'){
        ofSetWindowShape(640, 360);
         ofSetWindowPosition(0, ofGetScreenHeight()-ofGetWindowHeight());
    }else if(key == '2'){
        ofSetWindowShape(640, 400);
         ofSetWindowPosition(0, ofGetScreenHeight()-ofGetWindowHeight());
        
    }else if(key == '3'){
        ofSetWindowShape(640, 480);
         ofSetWindowPosition(0, ofGetScreenHeight()-ofGetWindowHeight());
        
    }else if(key == '4'){
        ofSetWindowShape(960, 540);
         ofSetWindowPosition(0, ofGetScreenHeight()-ofGetWindowHeight());
        
    }else if(key == '5'){
        ofSetWindowShape(1280, 720);
         ofSetWindowPosition(0, ofGetScreenHeight()-ofGetWindowHeight());
        
    }else if(key == ' '){
        bFullscreen = !bFullscreen;
        ofSetFullscreen(bFullscreen);
        if(!bFullscreen){
            ofSetWindowShape(640, 480);
        }
    }else if(key == 's'){
        mShader.load(ofToDataPath("displacement/shadersGL2/shader.vert"), ofToDataPath("displacement/shadersGL2/shader.frag"));
    }
    
   
//    if(key == OF_KEY_UP){
//        mISFPong.setEnable(mPongIndex, false);
//        mPongIndex--;
//        if(mPongIndex < 0){
//            mPongIndex = mISFPong.size()-1;;
//        }
//        mISFPong.setEnable(mPongIndex, true);
//    }
//    if(key == OF_KEY_DOWN){
//        mISFPong.setEnable(mPongIndex, false);
//        mPongIndex++;
//        if(mPongIndex >= mISFPong.size()){
//            mPongIndex =0;
//        }
//        mISFPong.setEnable(mPongIndex, true);
//    }
//    
//    if(key == OF_KEY_BACKSPACE){
//        ofFile::removeFile(files[mPongIndex]);
//    }
//    if(key == OF_KEY_RETURN){
//        ofFile::removeFile(files[mPingIndex]);
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    mViewPort = ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
