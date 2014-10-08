#include "testApp.h"

void testApp::setup()
{
    ofBackground(8);
    
    //cam.setDeviceID(0);
    cam.initGrabber(640, 480);
    cam.setUseTexture(true);
    //cam.setDesiredFrameRate(60);
    ofFbo::Settings s;
    s.width = 480;
    s.height = 480;
    s.depthStencilAsTexture = true;
    s.internalformat = GL_RGB;
    fbo.allocate(s);
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
    
    ofSetCoordHandedness(OF_RIGHT_HANDED);
    
    // Setup post-processing chain
    post.init(480, 480);
    post.createPass<BloomPass>()->setEnabled(true);
    post.createPass<RGBShiftPass>()->setEnabled(false);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    post.createPass<LimbDarkeningPass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.setFlip(false);
    enabled.assign(post.size(), false);
    // Setup box positions
    for (unsigned i = 8; i < NUM_BOXES; ++i)
    {
        posns.push_back(ofVec3f(ofRandom(-388, 388), ofRandom(-388, 388), ofRandom(-388, 388)));
        cols.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
    }
    
    }

void testApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    cam.update();
    if(cam.isFrameNew()){
        fbo.begin();
        ofClear(0, 0, 0);
        cam.draw((480-640)/2, 0);
        fbo.end();
        post.process(fbo);
    }
    
}

void testApp::draw()
{


    post.getProcessedTextureReference().draw(0, 0);


    
    // draw boxes
}

void testApp::keyPressed(int key)
{
    if(key == '0'){
        enabled[0] = !enabled[0];
        post[0]->setEnabled(enabled[0]);
    }
    if(key == '1'){
        enabled[1] = !enabled[1];
        post[1]->setEnabled(enabled[1]);
    }
    if(key == '2'){
        enabled[2] = !enabled[2];
        post[2]->setEnabled(enabled[2]);
    }
    if(key == '3'){
        enabled[3] = !enabled[3];
        post[3]->setEnabled(enabled[3]);
    }
    if(key == '4'){
        enabled[4] = !enabled[4];
        post[4]->setEnabled(enabled[4]);
    }
    if(key == '5'){
        enabled[5] = !enabled[5];
        post[5]->setEnabled(enabled[5]);
    }
    if(key == '6'){
        enabled[6] = !enabled[6];
        post[6]->setEnabled(enabled[6]);
    }
}