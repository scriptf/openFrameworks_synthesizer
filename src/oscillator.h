#pragma once
#include "ofMain.h"

class Oscillator {

public:
    int sampleRate;
    int periodDiscrete; // discrete signal period
    int periodCounter;
    float frequency;
    float phase;
    float phaseAdder;
    float phaseAdderTarget; // TWO_PI*f0/fs * n of s(n) = A * sin(TWO_PI*f0/fs * n)
    float volume;
    float leftScale;  // = 1 - pan;
	float rightScale; //  = pan;
  	float pan;        // 0.0 to 1.0
    int shape;
    string shapeName;
    string shapeStr;
    static enum WaveShape{SIN, RECT , SAW};

    vector <float> LTimeseries;
    vector <float> timeseries;
    
    int midiCcVolume;
    int midiCcPan;
    Oscillator(int bufferSize, int sampleRate, float frequency, float volume, float pan);
    ~Oscillator();
    float setFrequency(float frequency);
    float setVolume(float volume);
    float setPan(float pan);
    float setWaveShape(int shape);
    float setWaveShape(string shape);
    
    //float getSample(float phase); // サンプルを返す
    float getSample(); // サンプルを返す
    float getLSample(); // left sample
    float getRSample(); // right sample
    float getLGraph(int i);
    float getRGraph(int i);
    void updateTimeseries();

private:
};


