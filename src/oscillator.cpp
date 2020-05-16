#include "oscillator.h"
#include <string>
#include <algorithm> // for std::max_element

Oscillator::Oscillator(int bufferSize, int sampleRate,  float frequency, float volume, float pan)

    :sampleRate(sampleRate)
    ,periodDiscrete(floor((float)sampleRate/frequency))
    ,periodCounter(0)
    ,frequency(frequency)
    ,phase(0.0)
    ,phaseAdder(0.1f)
    ,phaseAdderTarget((frequency / (float) sampleRate)*TWO_PI)
    ,volume(volume)
    ,pan(pan)
    ,shape(0) // 0: sin wave, 1: rect eave, 2: saw wave
    ,shapeStr("Sin")
    ,shapeName("Sin")
{
//    timeseries.assign(bufferSize, 0.0);
    timeseries.assign(sampleRate, 0.0);
    enum WaveShape {SIN, RECT, SAW};
    leftScale = 1.0 - pan;
    rightScale = pan;

    ofLog(OF_LOG_NOTICE, "============================");
    ofLog(OF_LOG_NOTICE, "frequency: %f", frequency);
    ofLog(OF_LOG_NOTICE, "periodDiscrete: %d", this->periodDiscrete);
    ofLog(OF_LOG_NOTICE, "shape: %d", this->shape);
    updateTimeseries();

}

Oscillator::~Oscillator()
{
    vector<float>().swap(timeseries);
}

float Oscillator::setFrequency(float frequency)
{
    // Preventing the period from diverging into an infinite number of
    if(frequency < 1)
    {
        frequency = 1.0;
    }
    this->frequency = frequency;
    this->phaseAdderTarget = (this->frequency / (float)this->sampleRate) * TWO_PI;

    periodDiscrete = floor((float)sampleRate/frequency); // Period when discretized
    updateTimeseries();
    //ofLog(OF_LOG_NOTICE, "frequency: %f", this->frequency);
    //ofLog(OF_LOG_NOTICE, "periodDiscrete: %d", this->periodDiscrete);
}

/*
 * Update drawing when manipulating "volume"
 * */
void Oscillator::updateTimeseries()
{
    this->phase = 0.0;       // Draw a waveform from (0,0)
    this->periodCounter = 0;
}

float Oscillator::setVolume(float volume)
{
    this->volume = volume * 0.5; // volume Max. is 0.5.
    updateTimeseries();
}

float Oscillator::setPan(float pan)
{
    this->pan = pan;
    this->leftScale = 1.0 - this->pan;
    this->rightScale = this->pan;
    updateTimeseries();
}

float Oscillator::setWaveShape(int shape)
{
    this->shape = shape;
    switch(shape)
    {
    case WaveShape::SIN :
        shapeName = "Sin";
        break;
    case WaveShape::RECT:
        shapeName = "Rect";
        break;
    case WaveShape::SAW:
        shapeName = "Saw";
        break;
    default:
        shapeName = "Sin";
        break;
    }
    updateTimeseries();
}

/**
 * sin(phase) generates normalized wave -1.0 to 1.0.
 * "result" is between -1.0 to 1.0.
 */
float Oscillator::getSample()
{
    float result = 0.0;
    // https://blog.csdn.net/weixin_45360983/article/details/100125041
    //enum WaveShape: int{SIN=0, RECT , SAW};
    //enum WaveShape: int{SIN=0, RECT , SAW};

    switch(shape)
    {
    case WaveShape::SIN :
        result = sin(phase);
        break;
    case WaveShape::RECT:
        result = sin(phase)>0 ? 1: -1;
        break;
    case WaveShape::SAW:
        result = 2.0*(phase/TWO_PI - floor(1.0/2.0 + phase/TWO_PI));
        break;
    default:
        result = sin(phase);
        break;
    }

    // Save time series data for one period
    if(periodCounter < sampleRate)
    {
        this->timeseries[periodCounter] = result;
        periodCounter++;
    }

    phase += phaseAdderTarget;
    // If you write the following with else, you will get noise, so the if statement was divided.
    if(phase > TWO_PI)
    {
        phase = 0.0;
    }
    return result;
}

float Oscillator::getLSample()
{
    return getSample() * this->volume * this->leftScale;
}

float Oscillator::getRSample()
{
    return getSample() * this->volume * this->rightScale;
}

// Draw left channel graph
float Oscillator::getLGraph(int i)
{
    return timeseries[i] * this->volume * this->leftScale;
}

// Draw right channel graph
float Oscillator::getRGraph(int i)
{
    return timeseries[i] * this->volume * this->rightScale;
}
