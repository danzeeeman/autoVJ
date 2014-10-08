/*
 *  ofxBeatDetector.cpp
 *  Rezanator
 *
 *  Created by Syed Reza Ali on 7/31/11.
 *  Copyright 2011 University of California, Santa Barbara. All rights reserved.
 *
 */

#include "ofxBeatDetector.h"

ofxBeatDetector::ofxBeatDetector()
{
    for(int i = 0; i < FFT_BINS; i++)
        fftSmoothed[i] = 0;
	
    for(int i = 0; i < FFT_SUBBANDS; i++)
    {
        for(int l = 0; l < ENERGY_HISTORY; l++)
		{
            energyHistory[i][l] = 0;
		}
        fftSubbands[i] = 0;
        averageEnergy[i] = 0;
    }
	historyPos = 0; 
    fftInit = true;
}

void ofxBeatDetector::update(float *fftRef)
{
	if(fftInit)
	{
		fft = fftRef; 
        for (int i = 0; i < FFT_BINS; i++)
        {
            // take the max, either the smoothed or the incoming:
            if (fftSmoothed[i] < fft[i]) 
			{
                fftSmoothed[i] = fft[i];
			}
            // let the smoothed value sink to zero:
            fftSmoothed[i] *= 0.90f;
        }
        
        if(detectBeats)
        {
            // calculate subbands
            for(int i = 0; i < FFT_SUBBANDS; i++)
            {
                fftSubbands[i] = 0;
                for(int b = 0; b < FFT_BINS/FFT_SUBBANDS; b++)
                {
                    fftSubbands[i] +=  fft[i*(FFT_BINS/FFT_SUBBANDS)+b];
                }
                fftSubbands[i] = fftSubbands[i]*(float)FFT_SUBBANDS/(float)FFT_BINS;
            }
            
            // calc average energy
            for(int i = 0; i < FFT_SUBBANDS; i++)
            {
                averageEnergy[i] = 0;
                for(int h = 0; h < ENERGY_HISTORY; h++)
                {
                    averageEnergy[i] += energyHistory[i][h];
                }
                averageEnergy[i] /= ENERGY_HISTORY;
            }
            
            // put new values into energy history
            for(int i = 0; i < FFT_SUBBANDS; i++)
            {				
                energyHistory[i][historyPos] = fftSubbands[i];
            }
            historyPos = (historyPos+1) % ENERGY_HISTORY; // forward pointer and rotate if necessary
        }
    }
}

bool ofxBeatDetector::isBeat(int subband)
{
    return fftSubbands[subband] > averageEnergy[subband]*beatValue;
}

bool ofxBeatDetector::isKick()
{
    return isBeat(0);
}

bool ofxBeatDetector::isSnare()
{
    int low = 1;
    int hi = FFT_SUBBANDS/3;
    int thresh = (hi-low)/3;
    return isBeatRange(low, hi, thresh);
}

bool ofxBeatDetector::isHat()
{
    int low = FFT_SUBBANDS/2;
    int hi = FFT_SUBBANDS-1;
    int thresh = (hi-low)/3;
    return isBeatRange(low, hi, thresh);
}


bool ofxBeatDetector::isHigh()
{
    int low = 4*(FFT_SUBBANDS/5);
    int hi = FFT_SUBBANDS-1; 
    int thresh = (hi-low)/5;
    return isBeatRange(low, hi, thresh);	
}

bool ofxBeatDetector::isMidHigh()
{
    int low = 3*(FFT_SUBBANDS/5);
    int hi = 4*(FFT_SUBBANDS/5); 
    int thresh = (hi-low)/5;
    return isBeatRange(low, hi, thresh);	
	
}
bool ofxBeatDetector::isMid()
{
    int low = 2*(FFT_SUBBANDS/5); 
    int hi = 3*(FFT_SUBBANDS/5); 
    int thresh = (hi-low)/5;
    return isBeatRange(low, hi, thresh);		
}
bool ofxBeatDetector::isMidLow()
{
	int low = 1*(FFT_SUBBANDS/5);
    int hi = 2*(FFT_SUBBANDS/5); 
    int thresh = (hi-low)/5;
    return isBeatRange(low, hi, thresh);		
}
bool ofxBeatDetector::isLow()
{
    int low = 1; 
    int hi = (FFT_SUBBANDS/5); 
    int thresh = (hi-low)/5;
    return isBeatRange(low, hi, thresh);		
}


bool ofxBeatDetector::isBeatRange(int low, int high, int threshold)
{
    int num = 0;
    for(int i = low; i < high+1; i++) 
        if(isBeat(i)) 
            num++;
    return num > threshold;
}

void ofxBeatDetector::drawBeats()
{
	ofSetRectMode(OF_RECTMODE_CORNER); 	
    for (int i = 0; i < FFT_SUBBANDS; i++)
    {
		ofSetColor(0,255,30);
		ofRect(10+i*5, ofGetHeight()-10, 5, -fftSubbands[i]*200.0);
    }

}

void ofxBeatDetector::drawSmoothFFT()
{
	float offset = FFT_SUBBANDS*5+20; 
	ofSetRectMode(OF_RECTMODE_CORNER); 
    for (int i = 0; i < FFT_BINS; i++)
    {
		ofSetColor(0,255,30);		
		ofRect(offset+i*2, ofGetHeight()-10, 2, -fftSmoothed[i]*200.0);
    }
}

float *ofxBeatDetector::getSmoothedFFT()
{
	return fftSmoothed; 
}
