#pragma once
#include "ofMain.h"

class Wave {

public:
    int sampleRate;
    int periodDiscrete; // 離散信号の周期
    int periodCounter;  // 離散信号が一周することを監視する
    float frequency;
    float phase;
    float phaseAdder;
    float phaseAdderTarget; // s(n) = A * sin(TWO_PI*f0/fs * n) of TWO_PI*f0/fs * n
    float volume;     //  = 0.1f;
    float leftScale;  // = 1 - pan;
	float rightScale; //  = pan;
  	float pan;   // 0.0 to 1.0
    int shape;
    // lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
    // rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;
    vector <float> timeseries;
    
    int midiCcVolume;
    int midiCcPan;
    //Wave(int bufferSize, float frequency_, int midiCcVolume_, int midiCcPan_);
    Wave(int bufferSize, int sampleRate, float frequency, int midiCcVolume, int midiCcPan);
    float setFrequency(float frequency);
    //float getSample(float phase); // サンプルを返す
    float getSample(); // サンプルを返す
private:

    /*
    enum Shape {
    	SIN, RECTANGLE
	};
    */
};


