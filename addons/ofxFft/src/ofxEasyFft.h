#pragma once
#include "ofMain.h"
#include "ofxFft.h"

class ofxEasyFft : public ofBaseSoundInput{
public:
	ofxEasyFft();
    ~ofxEasyFft();
	void setup(int bufferSize = 512,
						 fftWindowType windowType = OF_FFT_WINDOW_HAMMING,
						 fftImplementation implementation = OF_FFT_FFTW,
						 int audioBufferSize = 512,
						 int audioSampleRate = 48000);
	void setUseNormalization(bool useNormalization);
	void update();
	vector<float>& getAudio();
	vector<float>& getBins();
	
    void audioIn( float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );
    
    void audioIn( float * input, int bufferSize, int nChannels );
    
    void audioReceived( float * input, int bufferSize, int nChannels );

	
	ofxFft* fft;
	ofRtAudioSoundStream* stream;
private:
	bool useNormalization;
	ofMutex soundMutex;
	vector<float> audioFront, audioMiddle, audioBack, audioRaw;
	vector<float> bins, left, right;
	void normalize(vector<float>& data);

};
