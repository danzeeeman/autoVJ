

#include "ProcessBPM_MIDI.h"

//ofEvent<float> ProcessBPM_MIDI::beatBroadcast = ofEvent<float>();

void ProcessBPM_MIDI::setup(){
    // print input ports to console
	midiIn.listPorts(); // via instance
	midiIn.openPort("IAC Driver Bus 1");	// open a virtual port
    
	midiIn.ignoreTypes(false, false, false);
	// add testApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(false);
    
    graphBPM.assign(60, 0.0); //for keeping a small set of BPMs to average with
    graphSmoothBPM.assign(600, 0.0);
    
    prevTime = 0;
}
//----------------
void ProcessBPM_MIDI::update(){
    
}
//--------------------------------------------------------------
void ProcessBPM_MIDI::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    
    //    cout<<"status : "<<msg.status<<endl;
    //    cout<<"pitch : "<<msg.pitch<<endl;
    //    cout<<"value : "<<msg.value<<endl;
    //    cout<<"velocity : "<<msg.velocity<<endl;
//    if(msg.status==248){ //receive the midi clock ticks
//        cout<<"pitch : "<<msg.pitch<<endl;
//        cout<<"value : "<<msg.value<<endl;
//        cout<<"velocity : "<<msg.velocity<<endl;
//        cout<<msg.toString()<<endl;
//        bpm = 60000.0/(ofGetElapsedTimeMillis()-prevTime)/24; //calculate BPM
//        prevTime = ofGetElapsedTimeMillis();
//        
//        graphBPM.push_back(bpm); //add it to the vector
//        
//        avgBPM =0;
//        for(int i=0; i<graphBPM.size(); i++){
//            avgBPM = avgBPM+graphBPM[i];
//        }
//        
//        avgBPM = avgBPM/graphBPM.size();
//        
//        cout<<avgBPM<<endl;
//        
//        //        //make smoothing graph
//        //        graphSmoothBPM.push_back(avgBPM);
//        //
//        //        prevBPM = bpm;
//        //
//        ////        if(graphBPM.size() >=60){
//        ////            graphBPM.erase(graphBPM.begin(),graphBPM.begin()+1);
//        ////        }
//        ////
//        ////        if(graphSmoothBPM.size() >=600){
//        ////            graphSmoothBPM.erase(graphSmoothBPM.begin(),graphSmoothBPM.begin()+1);
//        ////        }
//    }
    
    if (msg.pitch==66&&midiMessage.velocity==127) { //wavetick sends the messages as note ons - so ignore the note off's with velocity 0
        float timeGrab = ofGetElapsedTimef();
        ofNotifyEvent(midiAtom, timeGrab ,this);
        //cout<<"beat"<<endl;
    }
    
    
    if (msg.pitch==65&&midiMessage.velocity==127) { //wavetick sends the messages as note ons - so ignore the note off's with velocity 0
        float timeGrab = ofGetElapsedTimef();
        //cout<<"beat"<<endl;
        avgBPM += (timeGrab - prevBPM);
        avgBPM = avgBPM/2.;
        prevBPM = timeGrab;
        ofNotifyEvent(everyBeat, timeGrab ,this);
        
    }
    
    if (msg.pitch==64&&midiMessage.velocity==127) {
        tickCounter=0;
        float timeGrab = ofGetElapsedTimef();
        avgClock += (timeGrab - preClock);
        avgClock = avgClock/2.0;
        preClock = timeGrab;
        ofNotifyEvent(firstBeat, timeGrab ,this);
    }
}
//----------------
float ProcessBPM_MIDI::getMeasurePosition(){

    return (ofGetElapsedTimef() - preClock)/(avgClock+1);
}

//----------------
float ProcessBPM_MIDI::getBPM(){
    return 60./(avgBPM);
}
