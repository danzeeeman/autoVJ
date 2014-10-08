#ifndef OFX_BEAT_DETECTOR
#define OFX_BEAT_DETECTOR

#include "ofMain.h"
#include <list>

#define FFT_BINS 256
#define FFT_SUBBANDS 32
#define ENERGY_HISTORY 43

using namespace std;

class ofxBeatDetector
{
protected:
    // fft & beat detection
	float fftSmoothed[FFT_BINS];
	float fftSubbands[FFT_SUBBANDS];
	float averageEnergy[FFT_SUBBANDS];
	float energyHistory[FFT_SUBBANDS][ENERGY_HISTORY];
	float *fft;
	float beatValue;
	int historyPos;
	bool detectBeats;
	
	bool fftInit;
	
public:
	ofxBeatDetector();
	virtual ~ofxBeatDetector() {}
	
    // FFT functions
    void update(float *fftRef);
	void drawBeats(); 
    void drawSmoothFFT(); 
	
	// detect beats
    void enableBeatDetect() {detectBeats = true;}    
    bool isBeat(int subband);
    bool isKick();
    bool isSnare();
    bool isHat();
	
	bool isHigh(); 
	bool isMidHigh(); 
	bool isMid(); 
	bool isMidLow(); 	
	bool isLow();
	
    bool isBeatRange(int low, int high, int threshold);
    void setBeatValue(float bv) {beatValue = bv;}
	float &getBeatValue() { return beatValue; }
	float *getSmoothedFFT(); 
    
};

#endif
