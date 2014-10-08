#include "ofMain.h"

#include "ofxISF.h"

class ofApp : public ofBaseApp
{
public:
	
	ofxISF::Chain chain;
	ofVideoGrabber video;
	int index;
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
		ofSetLogLevel(OF_LOG_VERBOSE);
		video.initGrabber(1280, 720);
		
		chain.setup(1280, 720);
		ofDirectory dir;
        dir.allowExt("fs");
        dir.allowExt("vs");
        
        //         This is the local ISF folder in bin/data/ISF
        dir.listDir(ofToDataPath("ISF-Folder"));
        
        //        use the following line for the VDMX ISF directory
        //dir.listDir("/Library/Graphics/ISF");
        
    
        dir.sort();
        int count = 0;
        for(int i = 0; i < dir.size(); i++){
            // Multi Pass Gaussian and Layer Masks are currently broken in OF
            if(!ofIsStringInString(dir.getPath(i), "Multi Pass Gaussian") && !ofIsStringInString(dir.getPath(i), "Layer Mask")){
                if(chain.load(dir.getPath(i))){
                    chain.setEnable(count, false);
                    count++;
                }
            }
        }
        index = 0;
        chain.setImage(video.getTextureReference());
	}
	
	void update()
	{
        video.update();
		chain.update();
        ofSetWindowTitle(ofToString(ofGetFrameRate()));
	}
	
	void draw()
	{
		chain.draw(0, 0);
	}
    
	void keyPressed(int key)
	{
        if(key == OF_KEY_LEFT){
            chain.setEnable(index, false);
            index--;
            if(index < 0){
                index = chain.size()-1;;
            }
            chain.setEnable(index, true);
        }else if(key == OF_KEY_RIGHT){
            chain.setEnable(index, false);
            index++;
            if(index >= chain.size()){
                index = 0;
            }
            chain.setEnable(index, true);
        }
    }
    
    
    void keyReleased(int key)
    {
        
    }
    
    void mouseMoved(int x, int y)
    {
    }
    
    void mouseDragged(int x, int y, int button)
    {
    }
    
    void mousePressed(int x, int y, int button)
    {
        
    }
    
    void mouseReleased(int x, int y, int button)
    {
    }
    
    void windowResized(int w, int h)
    {
    }
};

int main(int argc, const char** argv)
{
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp);
    return 0;
}
