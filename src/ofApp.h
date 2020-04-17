#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "oscillator.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
	public:

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void audioOut(ofSoundBuffer & buffer);
				
		ofSoundStream soundStream;

		int		sampleRate;
		bool 	bNoise;
		float 	volume;

		float 	pan;
		float   leftScale;
		float   rightScale;
	
		int countSoundFrame;
		vector <float> lAudio;
		vector <float> rAudio;

		Oscillator* osci1;
		Oscillator* osci2;
		Oscillator* osci_sum;
		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;

		//-------------------  midi
		void newMidiMessage(ofxMidiMessage& eventArgs);
		
		ofxMidiIn midiIn;
//		std::vector<ofxMidiMessage> midiMessages;
		std::queue<ofxMidiMessage> midiMessages; // good container for FIFO


		std::size_t maxMessages = 10; //< max number of messages to keep track of

		ofFile myTextFile;
	
};

enum MidiCC {
	Saturday, Sunday, Tuesday, Wednesday, Thursday, Friday
	};
