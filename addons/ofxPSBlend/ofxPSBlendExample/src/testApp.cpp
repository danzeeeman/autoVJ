#include "testApp.h"

void testApp::setup()
{
    base.loadImage("imgs/7_frames/IMG2_0008.jpg");
    target7.loadImage("imgs/7_frames/IMG2_0001.jpg");
    target6.loadImage("imgs/7_frames/IMG2_0002.jpg");
    target5.loadImage("imgs/7_frames/IMG2_0003.jpg");
    target4.loadImage("imgs/7_frames/IMG2_0004.jpg");
    target3.loadImage("imgs/7_frames/IMG2_0005.jpg");
    target2.loadImage("imgs/7_frames/IMG2_0006.jpg");
    target1.loadImage("imgs/7_frames/IMG2_0007.jpg");
    
    ofSetWindowShape(target1.getWidth(), target1.getHeight());
 
    
    psBlend.setup(target1.getWidth(), target1.getHeight());
    blendMode = 0;
}

void testApp::update()
{
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    psBlend.begin();
    ofSetColor(255, 255, 255, 75);
    target1.draw(0, 0);
    ofSetColor(255, 255, 255, 75);
    target2.draw(0, 0);
    ofSetColor(255, 255, 255, 75);
    target3.draw(0, 0);
    ofSetColor(255, 255, 255, 75);
    target4.draw(0, 0);
    ofSetColor(255, 255, 255, 75);
    target5.draw(0, 0);
    ofSetColor(255, 255, 255, 75);
    target6.draw(0, 0);
    ofSetColor(255, 255, 255, 75);
    target1.draw(0, 0);
    psBlend.end();
}

void testApp::draw()
{
    //psBlend.draw(target2.getTextureReference(), blendMode);
    
    psBlend.draw(base.getTextureReference(), blendMode);
    ofSetWindowTitle("blendMode: " + psBlend.getBlendMode(blendMode));
    ofDrawBitmapString("press UP/DOWN key", 10, 20);
}

void testApp::keyPressed(int key)
{
    if (key == OF_KEY_UP)
    {
        if (blendMode >= 24)
        {
            blendMode = 0;
        }
        else {
            blendMode++;
        }
    }
    if (key == OF_KEY_DOWN)
    {
        if (blendMode <= 0)
        {
            blendMode = 24;
        }
        else
        {
            blendMode--;
        }
    }
    if (key == ' ')
    {
        ofSaveFrame();
    }
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}