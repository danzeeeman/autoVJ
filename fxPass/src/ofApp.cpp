#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    processor.init();
    sphere.set(100, 10);
    box.set(100, 100, 100);
    sphere.setPosition(-50, 0, 0);
    box.setPosition(50, 0, 0);

    lightPass = new ofxDeferred::DeferredLightingPass
}

//--------------------------------------------------------------
void ofApp::update(){
    DeferredEffect::DeferredLight light;
    light.specularColor = ofRandom(1.0);
    light.ambientColor = ofRandom(1.0);
    light.diffuseColor = ofRandom(0.2);

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    processor.begin(cam);
    sphere.draw();
    box.draw();
    processor.end();
    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
