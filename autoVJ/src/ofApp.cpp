#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    ofSetFullscreen(false);
    //ofSetDataPathRoot("../Resources/data/");
    mViewPort = ofRectangle(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    
    mFFt.setup();
    mFFt.setNormalize(true);
    mFFt.setExponent(1.25);
    mFFt.setFFTpercentage(0.25);
    mFFt.setNumFFTBins(256);
    
    mBeat = new ofxBeatDetector();
    mBeat->enableBeatDetect();
    mBeat->setBeatValue(1.25);
    mShiftTimer = ofGetElapsedTimef();
    
    debug = true;
    mixPing = 125;
    mixPong = 125;
    
    mVideoPing.setUseTexture(true);
    mVideoPong.setUseTexture(true);
    mVideoPing.loadMovie(ofToDataPath("video.mov"));
    mVideoPong.loadMovie(ofToDataPath("video.mov"));
    mVideoPing.setSpeed(1.5);
    mVideoPong.setSpeed(0.5);
    
    
    mVideoPong.play();
    mVideoPing.play();
    mVideoPing.setLoopState(OF_LOOP_NORMAL);
    mVideoPong.setLoopState(OF_LOOP_NORMAL);
    
    mVideoPing.setVolume(0);
    mVideoPong.setVolume(0);
    
    blendModeOne = 0;
    blendModeTwo = 0;
    
    
    allocateFbos();
    
}
void ofApp::allocateFbos(){
    mFrame.allocate(mViewPort.width, mViewPort.height, GL_RGBA, 4);
    mPing.allocate(mViewPort.width, mViewPort.height, GL_RGBA, 4);
    mPong.allocate(mViewPort.width, mViewPort.height, GL_RGBA, 4);
    mMasker.allocate(mViewPort.width, mViewPort.height, GL_RGBA, 4);
    mBackColor.allocate(mViewPort.width, mViewPort.height, GL_RGBA, 4);
    mDebugFrame.allocate(mViewPort.width, mViewPort.height, GL_RGBA, 4);
    
    mFrame.begin();
    ofClear(0, 0, 0, 0);
    mFrame.end();
    
    mDebugFrame.begin();
    ofClear(0, 0, 0, 0);
    mDebugFrame.end();
    
    mPing.begin();
    ofClear(0, 0, 0, 0);
    mPing.end();
    
    
    mPong.begin();
    ofClear(0, 0, 0, 0);
    mPong.end();
    
    mMasker.begin();
    ofClear(0, 0, 0, 0);
    mMasker.end();
    
    mBackColor.begin();
    ofClear(0, 0, 0, 0);
    mBackColor.end();
    
    
    
    
    blendOne.setup(mViewPort.width, mViewPort.height);
    blendTwo.setup(mViewPort.width, mViewPort.height);
    
    initShaderChains();
}
void ofApp::initShaderChains(){
    
    string shaderProgram = STRINGIFY(
                                     uniform sampler2DRect tex0;
                                     uniform sampler2DRect tex1;
                                     uniform sampler2DRect tex2;
                                     uniform sampler2DRect maskTex;
                                     
                                     void main (void){
                                         vec2 pos = gl_TexCoord[0].st;
                                         
                                         vec4 rTxt = texture2DRect(tex0, pos);
                                         vec4 gTxt = texture2DRect(tex1, pos);
                                         vec4 bTxt = texture2DRect(tex2, pos);
                                         vec4 mask = texture2DRect(maskTex, pos);
                                         
                                         vec4 color = vec4(0.0);
                                         color = mix(color, rTxt, mask);
                                         color = mix(color, gTxt, mask);
                                         color = mix(color, bTxt, mask);
                                         gl_FragColor = color;
                                     });
    
    mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    mShader.linkProgram();
    
    
    
    
    mChainPing.setup(mViewPort.width, mViewPort.height);
    mChainPong.setup(mViewPort.width, mViewPort.height);
    mISFPing.setup(mViewPort.width, mViewPort.height);
    mISFPong.setup(mViewPort.width, mViewPort.height);
    
    dir.allowExt("fs");
    dir.allowExt("vs");
    dir.listDir(ofToDataPath("isf"));
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
    dir.listDir(ofToDataPath("chains/one"));
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
    dir.listDir(ofToDataPath("chains/two"));
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


    
    mPongIndex = 0;
    mPingIndex = 0;
    mISFPing.setEnable(mPingIndex, true);
    mISFPong.setEnable(mPongIndex, true);
    
    mISFPing.setImage(mPong.getTextureReference());
    mISFPong.setImage(mPing.getTextureReference());
    
    mChainPong.setImage(mPing.getTextureReference());
    mChainPing.setImage(mPong.getTextureReference());
    
    
    mGlitch.setup(&mFrame);
    mGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    mGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
    mGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
    mGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    mGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
    mGlitch.setFx(OFXPOSTGLITCH_OUTLINE, false);
    mGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
    mGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    mGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
    mGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
    mGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    mGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    mGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    mGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    mGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    mGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    
    mGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    
    
    mDebugGlitch.setup(&mDebugFrame);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_OUTLINE, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    
    mDebugGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    
    mGlitchPong.setup(&mPong);
    mGlitchPong.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
    mGlitchPong.setFx(OFXPOSTGLITCH_GLOW, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_SHAKER, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    mGlitchPong.setFx(OFXPOSTGLITCH_TWIST, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_OUTLINE, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_NOISE, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_SWELL, true);
    mGlitchPong.setFx(OFXPOSTGLITCH_INVERT, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    mGlitchPong.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    
    mGlitchPong.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    
    mGlitchPing.setup(&mPing);
    mGlitchPing.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_GLOW, true);
    mGlitchPing.setFx(OFXPOSTGLITCH_SHAKER, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_TWIST, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_OUTLINE, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_NOISE, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_SLITSCAN, true);
    mGlitchPing.setFx(OFXPOSTGLITCH_SWELL, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_INVERT, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    mGlitchPing.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    
    mGlitchPing.setFx(OFXPOSTGLITCH_INVERT, false);
    
}
//--------------------------------------------------------------
void ofApp::update(){
    mFFt.update();
    mBeat->update(&mFFt.getSpectrum()[0]);
    mVideoPong.update();
    mVideoPing.update();
    mAlpha = (((mFFt.getSuperLowVal()+mFFt.getMidVal()+mFFt.getLowVal()+mFFt.getHighVal())/4.0)+mAlpha)/2.0;
    
    
    vals[0] = (vals[0]+mFFt.getLowVal())/2.0;
    vals[1] =(vals[1]+ mFFt.getMidVal())/2.0;
    vals[2] = (vals[2]+mFFt.getHighVal())/2.0;
    vals[3] = (vals[3]+mFFt.getSuperLowVal())/2.0;
//    
    mChainPong.getShader("4Kaleidoscope Tile")->setUniform<float>("sides", ofMap(mFFt.getMidVal(), 0, 1, 6.0, 32.0, true));
    mChainPong.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide1", ofVec2f(ofMap(mFFt.getHighVal(), 0, 1, -1.0, 1.0, true), ofMap(mFFt.getHighVal(), 0, 1, -1.0, 1.0, true)));
    mChainPong.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide2", ofVec2f(ofMap(mFFt.getHighVal(), 0, 1, -1.0, 1.0, true), ofMap(mFFt.getHighVal(), 0, 1, -1.0, 1.0, true)));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<float>("sides", ofMap(mFFt.getMidVal(), 0, 1, 6.0, 32.0, true));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide1", ofVec2f(ofMap(mFFt.getLowVal(), 0, 1, -1.0, 1.0, true), ofMap(mFFt.getLowVal(), 0, 1, -1.0, 1.0, true)));
    mChainPing.getShader("4Kaleidoscope Tile")->setUniform<ofVec2f>("slide2", ofVec2f(ofMap(mFFt.getLowVal(), 0, 1, -1.0, 1.0, true), ofMap(mFFt.getLowVal(), 0, 1, -1.0, 1.0, true)));
    //gridSize
    mChainPong.getShader("5CMYK Halftone-Lookaround")->setUniform<float>("gridSize", ofMap(mFFt.getHighVal(), 0, 1, 1.0, 15.0, true));
    mChainPong.getShader("5CMYK Halftone-Lookaround")->setUniform<float>("smoothing", ofMap(mFFt.getHighVal(), 0, 1, 1.0, 0.0, true));
    mChainPong.getShader("5CMYK Halftone")->setUniform<float>("gridSize", ofMap(mFFt.getMidVal(), 0, 1, 1.0, 15.0, true));
    mChainPong.getShader("5CMYK Halftone")->setUniform<float>("smoothing", ofMap(mFFt.getHighVal(), 0, 1, 1.0, 0.0, true));

    
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("sides", ofMap(mFFt.getMidVal(), 0, 1, 6.0, 32.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("slidey",ofMap(mFFt.getHighVal(), 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("angle",ofMap(mFFt.getMidVal(), 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<float>("slidex", ofMap(mFFt.getHighVal(), 0, 1, 0.0, 1.0, true));
    mChainPong.getShader("4Kaleidoscope")->setUniform<ofVec2f>("center", ofVec2f(mChainPing.getWidth()/2, mChainPing.getHeight()/2));
    
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("sides", ofMap(mFFt.getMidVal(), 0, 1, 6.0, 32.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("angle",ofMap(mFFt.getHighVal(), 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("slidey",ofMap(mFFt.getHighVal(), 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<float>("slidex", ofMap(mFFt.getMidVal(), 0, 1, 0.0, 1.0, true));
    mChainPing.getShader("5Kaleidoscope")->setUniform<ofVec2f>("center", ofVec2f(mChainPing.getWidth()/2,  mChainPing.getHeight()/2));
    
    
    
    
    mChainPong.getShader("9Color Controls")->setUniform<float>("bright", 0.0);
    mChainPong.getShader("9Color Controls")->setUniform<float>("contrast",ofMap(mFFt.getHighVal(), 0, 1, 2.0, 4.0, true));
    mChainPong.getShader("9Color Controls")->setUniform<float>("hue",ofMap(mFFt.getMidVal(), 0, 1, -1.0, 1.0, true));
    mChainPong.getShader("9Color Controls")->setUniform<float>("saturation", ofMap(mFFt.getSuperLowVal(), 0, 1, 4.0, 0.0, true));


    
    mChainPing.getShader("9Color Controls")->setUniform<float>("bright", 0.0);
    mChainPing.getShader("9Color Controls")->setUniform<float>("contrast",ofMap(mFFt.getHighVal(), 0, 1, 2.0,4.0, true));
    mChainPing.getShader("9Color Controls")->setUniform<float>("hue",ofMap(mFFt.getHighVal(), 0, 1, -1.0, 1.0, true));
    mChainPing.getShader("9Color Controls")->setUniform<float>("saturation", ofMap(mFFt.getSuperLowVal(), 0, 1, 4.0, 0.0, true));

    
    
    mISFPing.update();
    mISFPong.update();
    mChainPing.update();
    mChainPong.update();
    

    
    //ofPushStyle();
    //ofEnableAlphaBlending();
    mPing.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 255);
    mISFPong.draw(0, 0, mViewPort.width, mViewPort.height);
    ofDisableAlphaBlending();
    mPing.end();
    
    
    
    mPong.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 255);
    mISFPing.draw(0, 0, mViewPort.width, mViewPort.height);
    ofDisableAlphaBlending();
    ofPopStyle();
    mPong.end();
    
    mGlitchPing.setShadeVal(&vals[0]);
    mGlitchPing.generateFx();
    
    mGlitchPong.setShadeVal(&vals[0]);
    mGlitchPong.generateFx();
    
    
    
//    blendOne.begin();
//    ofSetColor(255, 255);
//    mISFPing.draw(0, 0, mViewPort.width, mViewPort.height);
//    blendOne.end();
//    
//    
//    mPong.begin();
//    ofSetColor(255, 255);
//    mChainPong.draw(0,0);
//    mPong.end();
//
//    blendTwo.begin();
//    ofSetColor(255, 255);
//    mISFPong.draw(0, 0, mViewPort.width, mViewPort.height);
//    blendTwo.end();
//    
//    
//    mPing.begin();
//    mChainPing.draw(0,0);
//    mPing.end();
//
//    mGlitchPing.setShadeVal(&vals[0]);
//    mGlitchPing.generateFx();
//    
//    mGlitchPong.setShadeVal(&vals[0]);
//    mGlitchPong.generateFx();
//    
    
    
    mMasker.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, ofMap(mFFt.getHighVal(), 0, 1, 50, 125));
    mVideoPing.draw(-(mVideoPing.width-mViewPort.width)/2, -(mVideoPing.height-mViewPort.height)/2);
    ofSetColor(255, ofMap(mFFt.getSuperLowVal(), 0, 1, 50, 125));
    mVideoPong.draw(-(mVideoPing.width-mViewPort.width)/2, -(mVideoPing.height-mViewPort.height)/2);
    ofSetColor(255, ofMap(mFFt.getMidVal(), 0, 1, 50, 125));
    mVideoPing.draw(0,0);
    ofSetColor(255,  ofMap(mFFt.getLowVal(), 0, 1, 50, 125));
    mVideoPong.draw(0,0);

    ofDisableAlphaBlending();
    ofPopStyle();
    mMasker.end();
    
    mFrame.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    mShader.begin();
    // Pass the video texture
    mShader.setUniformTexture("tex0", mChainPong.getTextureReference() , 1 );
    // Pass the image texture
    mShader.setUniformTexture("tex1", mChainPing.getTextureReference(), 2 );
    // Pass the movie texture
    mShader.setUniformTexture("tex2", mBackColor.getTextureReference() , 3 );
    // Pass the mask texture
    mShader.setUniformTexture("maskTex", mMasker.getTextureReference() , 4 );
    ofSetColor(255,255);
    mMasker.draw(0, 0, mViewPort.width, mViewPort.height);
    
    mShader.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    mFrame.end();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    mBackColor.begin();
    ofClear(0, 0, 0, 0);


    mShader.begin();
    // Pass the video texture
    mShader.setUniformTexture("tex0", mChainPing.getTextureReference() , 1 );
    // Pass the image texture
    mShader.setUniformTexture("tex1", mChainPong.getTextureReference(), 2 );
    // Pass the movie texture
    mShader.setUniformTexture("tex2", mFrame.getTextureReference() , 3 );
    // Pass the mask texture
    mShader.setUniformTexture("maskTex", mMasker.getTextureReference() , 4 );
    ofSetColor(255,255);
    mMasker.draw(0, 0, mViewPort.width, mViewPort.height);
    
    mShader.end();

    mBackColor.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
   
    

    
    
    
    
    
    
    
    if(debug){
        
        drawDebug();
        
    }
    
    
    

    
    
    //ofDisableAlphaBlending();
    //ofPopStyle();
    
    //    float deltaThreshold = 0.10;
    //    if ((abs(mFFt.getDelta())>(mFFt.getSmoothedUnScaledLoudestValue()*0.05)) && mFFt.getUnScaledLoudestValue() > 0.1) {
    if(ofGetElapsedTimef() - mShiftTimer  > 2.0){
        mShiftTimer = ofGetElapsedTimef();
        if(mBeat->isMidLow()){
            mGlitchPing.toggleFx(OFXPOSTGLITCH_CONVERGENCE);
            mGlitchPong.toggleFx(OFXPOSTGLITCH_SHAKER);
            mGlitch.toggleFx(OFXPOSTGLITCH_CONVERGENCE);
            if(debug){
                mDebugGlitch.toggleFx(OFXPOSTGLITCH_CONVERGENCE);
            }
        }
        if(mBeat->isMidHigh()){
            mGlitchPing.toggleFx(OFXPOSTGLITCH_SHAKER);
            mGlitchPong.toggleFx(OFXPOSTGLITCH_CONVERGENCE);
              mGlitch.toggleFx(OFXPOSTGLITCH_SHAKER);
            if(debug){
                mDebugGlitch.toggleFx(OFXPOSTGLITCH_SHAKER);
            }
        }
        if(mBeat->isKick()){
            mGlitchPing.toggleFx(OFXPOSTGLITCH_CUTSLIDER);
             mGlitchPong.toggleFx(OFXPOSTGLITCH_SLITSCAN);
            mGlitch.toggleFx(OFXPOSTGLITCH_CUTSLIDER);
            if(debug){
                mDebugGlitch.toggleFx(OFXPOSTGLITCH_CUTSLIDER);
            }
        }
        if(mBeat->isMid()){
            mGlitchPing.toggleFx(OFXPOSTGLITCH_SLITSCAN);
            mGlitchPong.toggleFx(OFXPOSTGLITCH_CUTSLIDER);
            mGlitch.toggleFx(OFXPOSTGLITCH_SLITSCAN);

            if(debug){
                mDebugGlitch.toggleFx(OFXPOSTGLITCH_SLITSCAN);
            }
        }
        if(mBeat->isHigh()){
            mGlitchPing.toggleFx(OFXPOSTGLITCH_SWELL);
            mGlitchPong.toggleFx(OFXPOSTGLITCH_CONVERGENCE);
              mGlitch.toggleFx(OFXPOSTGLITCH_SWELL);
            if(debug){
                mDebugGlitch.toggleFx(OFXPOSTGLITCH_SWELL);
            }
        }
        if(mBeat->isLow()){
            mGlitchPing.toggleFx(OFXPOSTGLITCH_CR_GREENRAISE);
            mGlitchPing.toggleFx(OFXPOSTGLITCH_CR_BLUERAISE);
            mGlitchPing.toggleFx(OFXPOSTGLITCH_CR_REDRAISE);
            mGlitch.toggleFx(OFXPOSTGLITCH_CR_REDRAISE);
            //            mGlitch.toggleFx(OFXPOSTGLITCH_INVERT);
            if(debug){
                mGlitch.toggleFx(OFXPOSTGLITCH_CR_GREENRAISE);
                mGlitch.toggleFx(OFXPOSTGLITCH_CR_BLUERAISE);
                mGlitch.toggleFx(OFXPOSTGLITCH_CR_REDRAISE);
            }
        }
    }
    //    }
    if ((abs(mFFt.getDelta())>(mFFt.getSmoothedUnScaledLoudestValue()*0.22)) && mFFt.getUnScaledLoudestValue() > 0.1 && (ofGetElapsedTimef() - deltaShiftPong)>6.0 ) {
        deltaShiftPong = ofGetElapsedTimef();
        mISFPing.setEnable(mPingIndex, false);
        mPingIndex--;
        if(mPingIndex < 0){
            mPingIndex = mISFPing.size()-1;;
        }
        mISFPing.setEnable(mPingIndex, true);
        //}
        
        if (blendModeTwo >= 24)
        {
            blendModeTwo = 0;
        }
        else {
            blendModeTwo++;
        }
        
        if (blendModeOne < 0)
        {
            blendModeOne = 24;
        }
        else {
            blendModeOne--;
        }
        
    }
    if((abs(mFFt.getDelta())>(mFFt.getSmoothedUnScaledLoudestValue()*0.33)) && mFFt.getUnScaledLoudestValue() > 0.1 && (ofGetElapsedTimef() - deltaShiftPing)>6.0){
        deltaShiftPing = ofGetElapsedTimef();
        mISFPong.setEnable(mPongIndex, false);
        mPongIndex++;
        if(mPongIndex >= mISFPong.size()){
            mPongIndex =0;
        }
        mISFPong.setEnable(mPongIndex, true);
        
        if (blendModeOne >= 24)
        {
            blendModeOne = 0;
        }
        else {
            blendModeOne++;
        }
        
        if (blendModeTwo < 0)
        {
            blendModeTwo = 24;
        }
        else {
            blendModeTwo--;
        }
        
        
    }
    
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    mFrame.begin();
    ofClear(0, 0, 0, 0.0);
    
    //    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //    ofSetColor(255, ofMap(mFFt.getHighVal(), 0, 1, 0, 200));
    //    mMasker.draw(0, 0);
    
    

    ofSetColor(255,  ofMap(mFFt.getHighVal(), 0, 1, 10, 225,true));
    mChainPing.draw(0, 0);
    ofSetColor(255, ofMap(mFFt.getMidVal(), 0, 1, 10, 225, true));
    mChainPong.draw(0, 0);
    ofSetColor(255,  ofMap(mFFt.getSuperLowVal(), 0, 1, 10, 225,true));
    mBackColor.draw(0, 0);
//    ofSetColor(255, ofMap(mFFt.getLowVal(), 0, 1, 10, 225));
//    mMasker.draw(0,0);
    
//    mShader.begin();
//    // Pass the video texture
//    mShader.setUniformTexture("tex0", mChainPong.getTextureReference() , 1 );
//    // Pass the image texture
//    mShader.setUniformTexture("tex1", mChainPing.getTextureReference(), 2 );
//    // Pass the movie texture
//    mShader.setUniformTexture("tex2", mBackColor.getTextureReference() , 3 );
//    // Pass the mask texture
//    mShader.setUniformTexture("maskTex", mBackColor.getTextureReference() , 4 );
//    ofSetColor(255, 155);
//    mBackColor.draw(0, 0);
//     ofSetColor(255, 155);
//    mChainPing.draw(0, 0);
//     ofSetColor(255, 155);
//    mChainPong.draw(0, 0);
//    mShader.end();
    mFrame.end();
    

//    mGlitch.setShadeVal(&vals[0]);
//    mGlitch.generateFx();
    ofDisableAlphaBlending();
    
}
void ofApp::drawDebug(){
    mDebugFrame.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(mDebug);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    //mFFt.drawBars();
    mBeat->drawBeats();
    
    if(mBeat->isMidLow()){
        ofSetColor(0, 0, 255);
        ofRect(ofGetWidth()/4-250, ofGetHeight()/4-250, 500, 500);
    }
    if(mBeat->isMidHigh()){
        ofSetColor(0, 255, 255);
        ofRect(ofGetWidth()/4-200, ofGetHeight()/4-200, 400, 400);
    }
    if(mBeat->isKick()){
        ofSetColor(255, 255, 0);
        ofRect(ofGetWidth()/4-125, ofGetHeight()/4-125, 250, 250);
    }
    if(mBeat->isMid()){
        ofSetColor(0, 255, 0);
        ofRect(ofGetWidth()/4-100, ofGetHeight()/4-100, 200, 200);
    }
    if(mBeat->isHigh()){
        ofSetColor(255, 0, 255);
        ofRect(ofGetWidth()/4-75, ofGetHeight()/4-75, 150, 150);
    }
    if(mBeat->isLow()){
        ofSetColor(255, 0, 0);
        ofRect(ofGetWidth()/4-25, ofGetHeight()/4-25, 50, 50);
    }
    mDebugFrame.end();
    
    mDebugGlitch.setShadeVal(&vals[0]);
    mDebugGlitch.generateFx();
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    
    ofSetColor(255, 255);
    mFrame.draw(0, 0, ofGetWidth(), ofGetHeight());

    if(debug){
        ofSetColor(255, 255);
        mDebugFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
        mChainPong.draw(50, 50, mViewPort.width/5, mViewPort.height/5);
        mChainPing.draw(50+mViewPort.width/5, 50, mViewPort.width/5, mViewPort.height/5);
        mMasker.draw(50, 50+mViewPort.height/5, mViewPort.width/5, mViewPort.height/5);
        mBackColor.draw(50+mViewPort.width/5, 50+mViewPort.height/5, mViewPort.width/5, mViewPort.height/5);
        mFrame.draw(50+mViewPort.width/5*2, 50+mViewPort.height/5, mViewPort.width/5, mViewPort.height/5);
        
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("DEBUG MODE PRESS D TO GO LIVE", ofGetWidth()-300, ofGetHeight()-25);
        
        mBeat->drawSmoothFFT();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'd'){
        debug = !debug;
        ofSetFullscreen(!debug);
    }
    if(key == OF_KEY_LEFT){
        mISFPing.setEnable(mPingIndex, false);
        mPingIndex--;
        if(mPingIndex < 0){
            mPingIndex = mISFPing.size()-1;;
        }
        mISFPing.setEnable(mPingIndex, true);
    }
    if(key == OF_KEY_RIGHT){
        mISFPing.setEnable(mPingIndex, false);
        mPingIndex++;
        if(mPingIndex >= mISFPing.size()){
            mPingIndex =0;
        }
        mISFPing.setEnable(mPingIndex, true);
        
    }
    
    if(key == OF_KEY_UP){
        mISFPong.setEnable(mPongIndex, false);
        mPongIndex--;
        if(mPongIndex < 0){
            mPongIndex = mISFPong.size()-1;;
        }
        mISFPong.setEnable(mPongIndex, true);
    }
    if(key == OF_KEY_DOWN){
        mISFPong.setEnable(mPongIndex, false);
        mPongIndex++;
        if(mPongIndex >= mISFPong.size()){
            mPongIndex =0;
        }
        mISFPong.setEnable(mPongIndex, true);
    }
    
    if(key == OF_KEY_BACKSPACE){
        ofFile::removeFile(files[mPongIndex]);
    }
    
    if(key == '1'){
        mGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        
    }
    if(key == '2'){
        mGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
        
    }
    if(key == '3'){
        mGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
        
    }
    if(key == '4'){
        mGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        
    }
    if(key == '5'){
        mGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
        
    }
    if(key == '6'){
        mGlitch.setFx(OFXPOSTGLITCH_OUTLINE, true);
        
    }
    if(key == '7'){
        mGlitch.setFx(OFXPOSTGLITCH_NOISE, true);
        
    }
    if(key == '8'){
        mGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        
    }
    if(key == '9'){
        mGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
        
    }
    if(key == '0'){
        mGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
        
    }
    if(key == 'q'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    }
    if(key == 'w'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
    }
    if(key == 'e'){
        
        mGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
    }
    if(key == 'r'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
    }
    if(key == 't'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
    }
    if(key == 'y'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
    }
    if(key == 'u'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
    }
    
    if(key == '='){
        if (blendModeOne >= 24)
        {
            blendModeOne = 0;
        }
        else {
            blendModeOne++;
        }
        
        if (blendModeTwo > 24)
        {
            blendModeTwo = 0;
        }
        else {
            blendModeTwo++;
        }
    }
    if(key == '-'){
        if (blendModeOne < 0)
        {
            blendModeOne = 24;
        }
        else {
            blendModeOne--;
        }
        
        if (blendModeTwo < 0)
        {
            blendModeTwo = 0;
        }
        else {
            blendModeTwo--;
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '1'){
        mGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        
    }
    if(key == '2'){
        mGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
        
    }
    if(key == '3'){
        mGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
        
    }
    if(key == '4'){
        mGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        
    }
    if(key == '5'){
        mGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
        
    }
    if(key == '6'){
        mGlitch.setFx(OFXPOSTGLITCH_OUTLINE, false);
        
    }
    if(key == '7'){
        mGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
        
    }
    if(key == '8'){
        mGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        
    }
    if(key == '9'){
        mGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
        
    }
    if(key == '0'){
        mGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
        
    }
    if(key == 'q'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    }
    if(key == 'w'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    }
    if(key == 'e'){
        
        mGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    }
    if(key == 'r'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    }
    if(key == 't'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    }
    if(key == 'y'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    }
    if(key == 'u'){
        mGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    }
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
