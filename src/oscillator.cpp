#include "oscillator.h"
#include<algorithm> // for std::max_element

Oscillator::Oscillator(int bufferSize, int sampleRate,  float frequency, int midiCcVolume, int midiCcPan)
    :sampleRate(sampleRate)
    ,periodDiscrete(floor((float)sampleRate/frequency))
    ,periodCounter(0)
    ,frequency(frequency)
    ,phase(0.0)
    ,phaseAdder(0.1f)
    ,phaseAdderTarget((frequency / (float) sampleRate)*TWO_PI)
    ,volume(0.1f)
    ,pan(0.0)
    ,leftScale(1.0) // ここで 1.0 - pan とすると pan が初期化されていない場合にバグる
	,rightScale(0.0)
    ,midiCcVolume(midiCcVolume)
    ,midiCcPan(midiCcPan)
    ,shape(0) // sin wave, 1 is rectable
    ,shapeStr("Sin")
{
//    timeseries.assign(bufferSize, 0.0);
    timeseries.assign(sampleRate, 0.0);
   // enum WaveShape: int{SIN=0, RECT , SAW};
    enum WaveShape{SIN, RECT , SAW};
    //enum class WaveShape {SIN, RECT , SAW};
    //this.WaveShape: int{SIN=0, RECT , SAW};
    

    ofLog(OF_LOG_NOTICE, "============================");
    ofLog(OF_LOG_NOTICE, "aaaaaaaaaaaa: %f", (float)sampleRate/frequency);
   	ofLog(OF_LOG_NOTICE, "periodDiscrete: %d", this->periodDiscrete);
  	ofLog(OF_LOG_NOTICE, "shape: %d", this->shape);
    updateTimeseries();

}

Oscillator::~Oscillator(){
   	vector<float>().swap(timeseries);
}

float Oscillator::setFrequency(float frequency) {
    // 周期の計算が無限大に発散するのを防ぐ
    if(frequency < 1){
        frequency = 1.0;
    }
    this->frequency = frequency;
    this->phaseAdderTarget = (this->frequency / (float)this->sampleRate) * TWO_PI;

    periodDiscrete = floor((float)sampleRate/frequency); // 離散化した時の周期
    updateTimeseries();
    //phase = 0.0;
    //periodCounter = 0;   
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

float Oscillator::setVolume(float volume) {
    this->volume = volume * 0.5; // volume Max. is 0.5.
    updateTimeseries();
}

float Oscillator::setPan(float pan) {
    this->pan = pan;
    this->leftScale = 1.0 - this->pan;
    this->rightScale = this->pan;
    updateTimeseries();
}

float Oscillator::setWaveShape(int shape) {
    this->shape = shape;
    updateTimeseries();
}

float Oscillator::setWaveShape(string shape) {
    this->shapeStr = shape;
    updateTimeseries();
}

/**
 * sin(phase) generates normalized wave -1.0 to 1.0.
 * "result" is between -1.0 to 1.0.
 */
float Oscillator::getSample(){
    float result = 0.0;
    /*
    switch(shape){
        case 0:
            result = sin(phase);
            break;
        case 1:
           	result = sin(phase)>0 ? 1: -1;
            break;
        case 2:
           	result = 2.0*(phase/TWO_PI - floor(1.0/2.0 + phase/TWO_PI));
            break;           
        default:
            result = sin(phase);
            break;
    }
    */
        // https://blog.csdn.net/weixin_45360983/article/details/100125041
        //enum WaveShape: int{SIN=0, RECT , SAW};
    //enum WaveShape: int{SIN=0, RECT , SAW};


    switch(shape){
//        case static_cast<int>(WaveShape::SIN) :
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

    
    /*
    switch(shapeStr){
        case "Sin":
            result = sin(phase);
            break;
        case "Rect":
           	result = sin(phase)>0 ? 1: -1;
            break;
        case "Saw":
           	result = 2.0*(phase/TWO_PI - floor(1.0/2.0 + phase/TWO_PI));
            break;           
        default:
            result = sin(phase);
            break;
    }
    */


    // 一周期分の時系列データを保存しておく  
    if(periodCounter < sampleRate){
        this->timeseries[periodCounter] = result;
        periodCounter++;
    }

    phase += phaseAdderTarget;
    // 以下を else で書くとプツプツとノイズが入るのでif文を分けた。
    if(phase > TWO_PI){
        phase = 0.0;
    }
    return result;
}

float Oscillator::getLSample(){
    return getSample() * this->volume * this->leftScale;
}

float Oscillator::getRSample(){
    return getSample() * this->volume * this->rightScale;
}

// Draw left channel graph
float Oscillator::getLGraph(int i){
    //std::vector<int>::iterator iter = std::max_element(timseries.begin(), timeseries.end());
    //int maxElementIndex = *std::max_element(timeseries.begin(),timeseries.end());
    //ofLog(OF_LOG_NOTICE, "Max timeseries[i] = %f",timeseries[i]);
    return timeseries[i] * this->volume * this->leftScale;
}

// Draw right channel graph
float Oscillator::getRGraph(int i){
    return timeseries[i] * this->volume * this->rightScale;
}
