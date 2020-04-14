
#include "wave.h"

//Wave::Wave(int bufferSize, float frequency_, int midiCcVolume_, int midiCcPan_){
Wave::Wave(int bufferSize, int sampleRate,  float frequency, int midiCcVolume, int midiCcPan)
    :sampleRate(sampleRate)
    ,frequency(frequency)
    ,phase(0.0)
    ,phaseAdder(0.1f)
    ,phaseAdderTarget((frequency / (float) sampleRate)*TWO_PI)
    ,volume(0.45f)
    ,pan(0.0)
    ,leftScale(1.0 - pan)
	,rightScale(pan)
    ,midiCcVolume(midiCcVolume)
    ,midiCcPan(midiCcPan)
{
    timeseries.assign(bufferSize, 0.0);
    shape = 0; // sin wave, 1 is rectable
}

float Wave::setFrequency(float frequency) {
    this->frequency = frequency;
    this->phaseAdderTarget = (this->frequency / (float)this->sampleRate) * TWO_PI;
    //this->phaseAdder = 0.95f * this->phaseAdder + 0.05f * this->phaseAdderTarget;
    //this->phaseAdder = this->phaseAdderTarget;
}

float Wave::getSample(){
    // sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI)
	{
		phase = 0.0;
	}
    phase += phaseAdder;
    /*
	ofLog(OF_LOG_NOTICE, "phase: %f", phase); 
	ofLog(OF_LOG_NOTICE, "f: %f", frequency); 
	ofLog(OF_LOG_NOTICE, "t: %f", phase*frequency*TWO_PI); 
    */
    switch(shape){
        case 0:
            return sin(phase) * volume;
        case 1:
           	return sin(phase)>0 ? volume : -volume;
    }
    return sin(phase) * volume;
}
