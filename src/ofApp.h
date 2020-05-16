#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "oscillator.h"
#include "ofxGuiExtended.h"

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
        int lAudioSize;
        int rAudioSize;
        //--------- grpah
        int gX;
        int gY;
        int gWidth;  // Width of graph
        int gHeight;
        int gHeightCenter;
        //------------------- Oscillator
        Oscillator* osci1;
		Oscillator* osci2;
		Oscillator* osci3;
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
        //------------------- file
		//ofFile myTextFile;
        
    private:
        //------------------- ofxGuiExtended
        ofxGui gui;
        //ofxGuiPanel *panels[3]; // Unused
        ofxGuiPanel *panel1, *panel2, *panel3;
        ofParameterGroup oscillatorParameters[3];
        /*
        ofParameterGroup oscillator1Parameters; // Unused
        ofParameterGroup oscillator2Parameters; // Unused
        ofParameterGroup oscillator3Parameters; // Unused
        */
        ofParameter<bool> waveShapes[9];
        //ofxGuiGroup *waveShapeToggles1; // Unused
        ofxGuiGroup *waveShapeToggles[3];
        void setWaveShape1(int& index);
        void setWaveShape2(int& index);
        void setWaveShape3(int& index);
        void setVolume1(float& volume);
        void setVolume2(float& volume);
        void setVolume3(float& volume);
        //void setFrequency(ofParameter<float>& frequency);
        void setFrequency1(float& frequency);
        void setFrequency2(float& frequency);
        void setFrequency3(float& frequency);
        void setPan1(float& volume);
        void setPan2(float& volume);
        void setPan3(float& volume);

        ofParameter<string> activeName;
        ofParameter<int> activeIndex;
        ofParameter<float> volume1;
        ofParameter<float> volume2;
        ofParameter<float> volume3;
        ofParameter<float> frequency1;
        ofParameter<float> frequency2;
        ofParameter<float> frequency3;
        ofParameter<float> pan1;
        ofParameter<float> pan2;
        ofParameter<float> pan3;
        ofParameter<float> volumes[3];
        ofParameter<float> pans[3];
    
};
