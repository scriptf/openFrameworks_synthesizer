#include "oscillator.h"

Oscillator::Oscillator(int bufferSize, int sampleRate,  float frequency, int midiCcVolume, int midiCcPan)
    :sampleRate(sampleRate)
    ,periodDiscrete(floor((float)sampleRate/frequency))
    ,periodCounter(0)
    ,frequency(frequency)
    ,phase(0.0)
    ,phaseAdder(0.1f)
    ,phaseAdderTarget((frequency / (float) sampleRate)*TWO_PI)
    ,volume(0.3f)
    ,pan(0.0)
    ,leftScale(1.0 - pan)
	,rightScale(pan)
    ,midiCcVolume(midiCcVolume)
    ,midiCcPan(midiCcPan)
    ,shape(0) // sin wave, 1 is rectable
{
//    timeseries.assign(bufferSize, 0.0);
    LTimeseries.assign(sampleRate, 0.0);
    ofLog(OF_LOG_NOTICE, "============================");
    ofLog(OF_LOG_NOTICE, "aaaaaaaaaaaa: %f", (float)sampleRate/frequency);
   	ofLog(OF_LOG_NOTICE, "periodDiscrete: %d", this->periodDiscrete);
   	ofLog(OF_LOG_NOTICE, "shape: %d", this->shape);
}

float Oscillator::setFrequency(float frequency) {
    // 周期の計算が無限大に発散するのを防ぐ
    if(frequency < 1){
        frequency = 1.0;
    }
    this->frequency = frequency;
    this->phaseAdderTarget = (this->frequency / (float)this->sampleRate) * TWO_PI;

    periodDiscrete = floor((float)sampleRate/frequency); // 離散化した時の周期
    phase = 0.0;
    periodCounter = 0;   
	//ofLog(OF_LOG_NOTICE, "frequency: %f", this->frequency);
   	//ofLog(OF_LOG_NOTICE, "periodDiscrete: %d", this->periodDiscrete);
    //this->phaseAdder = 0.95f * this->phaseAdder + 0.05f * this->phaseAdderTarget;
    //this->phaseAdder = this->phaseAdderTarget;
}

/*
 * volume を操作したとき描画をアップデートする
 *
 * */
void Oscillator::updateTimeseries() {
    this->phase = 0.0;       // (0,0)から波形を描画する
    this->periodCounter = 0; 
}


/**
 * Generate normalized wave -1.0 to 1.0 .
 */
float Oscillator::getSample(){
    // sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	//if (phase > TWO_PI)

    /*
	ofLog(OF_LOG_NOTICE, "phase: %f", phase); 
	ofLog(OF_LOG_NOTICE, "f: %f", frequency); 
	ofLog(OF_LOG_NOTICE, "t: %f", phase*frequency*TWO_PI); 
    */
    float result = 0.0;
    switch(shape){
        case 0:
//            result = sin(phase) * volume;
            result = sin(phase) * this->volume * this->leftScale;
            break;
        case 1:
           	result = sin(phase)>0 ? this->volume * this->leftScale: -this->volume * this->leftScale;
           	//result = sin(phase)>0 ? 1 : -1;
            break;
        case 2:
           	result = 2.0*(phase/TWO_PI - floor(1.0/2.0 + phase/TWO_PI)) * this->volume * this->leftScale;
           	//result = sin(phase)>0 ? 1 : -1;
            break;           
        default:
//            result = sin(phase) * volume;
            result = sin(phase) * this->volume * this->leftScale;
            break;
    }

    phase += phaseAdderTarget;


    /*
    if(phase > TWO_PI){
        phase = 0.0;
    }
    else{
        phase += phaseAdderTarget;
    }
    */

    
    // 一周期分の時系列データを保存しておく  
    if(periodCounter < sampleRate){
        this->LTimeseries[periodCounter] = result * this->volume * this->leftScale;
        periodCounter++;
    }
    // ここを else で書くとプツプツとノイズが入るので。
    // if文を分けた

    if(phase > TWO_PI){
        phase = 0.0;
    }
    //ofLog(OF_LOG_NOTICE, "result: %f", result);

    //phase += phaseAdder;
    return result;
}
