#include "ofApp.h"
#include "oscillator.h"
#include "midi_controllers.h"
//#include "./lib/nameof.hpp"
#include <iostream>
//#include "lib/nameof.hpp"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    pan = 0.0;
    leftScale = 1.0 - pan;
    rightScale = pan;
    gY = 32+3*12;
    gWidth = 300; // Width of graph
    gHeight = 200;
    gHeightCenter = gHeight / 2;
    
    // GUI ------------------------------------------------------------
    // Oscillator1 ----------------------------------------------------
    panel1 = gui.addPanel();
    panel1->setPosition(32,gY+gHeight);
    panel1->setShowHeader(false);
    
    /*
    panels[0] = gui.addPanel();
    panels[0]->setPosition(32,gY+gHeight);
    panels[0]->setShowHeader(false);
    */
    
    oscillatorParameters[0].setName("Oscillator1");
    oscillatorParameters[0].add(waveShapes[0].set("Sin",true));
    oscillatorParameters[0].add(waveShapes[1].set("Rect",false));
    oscillatorParameters[0].add(waveShapes[2].set("Saw",false));
    // ofxGuiGroup
    /*
    waveShapeToggles1 = panel1->addGroup(oscillatorParameters[0]);
    waveShapeToggles1->setExclusiveToggles(true);
    waveShapeToggles1->setConfig(ofJson({{"type", "radio"}}));
    */
    ///*
    waveShapeToggles[0] = panel1->addGroup(oscillatorParameters[0]);
    waveShapeToggles[0]->setExclusiveToggles(true);
    waveShapeToggles[0]->setConfig(ofJson({{"type", "radio"}}));
    //*/
    
    /*
    volumes[0].set("Volume 1", 0.5, 0, 1);
    panel1->add(volumes[0]);
    volumes[0].addListener(this, &ofApp::setVolume1);
    */
    volume1.set("Volume 1", 0.1, 0, 1);
    panel1->add(volume1);
    volume1.addListener(this, &ofApp::setVolume1);

//    /*
    frequency1.set("Frequency 1", 400, 0, 2000);
    panel1->add(frequency1);
    frequency1.addListener(this, &ofApp::setFrequency1);
//     */
/*
    frequency1.set("Frequency 1", 400, 0, 2000);
    panel1->add(frequency1);
    frequency1.addListener(this, &ofApp::setFrequency);
*/
    pan1.set("Pan 1", 0, 0, 1);
    panel1->add(pan1);
    pan1.addListener(this, &ofApp::setPan1);

     // Oscillator2 ----------------------------------------------------
    panel2 = gui.addPanel();
    panel2->setPosition(32+gWidth, gY+gHeight);
    panel2->setShowHeader(false);
    /*
    panels[1] = gui.addPanel();
    panels[1]->setPosition(32+gWidth, gY+gHeight);
    panels[1]->setShowHeader(false);
    */
    oscillatorParameters[1].setName("Oscillator2");
    oscillatorParameters[1].add(waveShapes[3].set("Sin",true));
    oscillatorParameters[1].add(waveShapes[4].set("Rect",false));
    oscillatorParameters[1].add(waveShapes[5].set("Saw",false));
    /*
    waveShapeToggles1 = panel2->addGroup(oscillatorParameters[1]);
    waveShapeToggles1->setExclusiveToggles(true);
    waveShapeToggles1->setConfig(ofJson({{"type", "radio"}}));
    */
    ///*
    waveShapeToggles[1] = panel2->addGroup(oscillatorParameters[1]);
    waveShapeToggles[1]->setExclusiveToggles(true);
    waveShapeToggles[1]->setConfig(ofJson({{"type", "radio"}}));
    //*/
    volume2.set("Volume 2", 0.1, 0, 1);
    panel2->add(volume2);
    volume2.addListener(this, &ofApp::setVolume2);

    frequency2.set("Frequency 2", 800, 0, 2000);
    panel2->add(frequency2);
    frequency2.addListener(this, &ofApp::setFrequency2);
    
    pan2.set("Pan 2", 0, 0, 1);
    panel2->add(pan2);
    pan2.addListener(this, &ofApp::setPan2);

    // Oscillator3 ----------------------------------------------------
    panel3 = gui.addPanel();
    panel3->setPosition(32+2*gWidth, gY+gHeight);
    panel3->setShowHeader(false);
    /*
    panels[1] = gui.addPanel();
    panels[1]->setPosition(32+gWidth, gY+gHeight);
    panels[1]->setShowHeader(false);
    */
    oscillatorParameters[2].setName("Oscillator3");
    oscillatorParameters[2].add(waveShapes[6].set("Sin",true));
    oscillatorParameters[2].add(waveShapes[7].set("Rect",false));
    oscillatorParameters[2].add(waveShapes[8].set("Saw",false));
    /*
    waveShapeToggles1 = panel3->addGroup(oscillatorParameters[2]);
    waveShapeToggles1->setExclusiveToggles(true);
    waveShapeToggles1->setConfig(ofJson({{"type", "radio"}}));
    */
    ///*
    waveShapeToggles[2] = panel3->addGroup(oscillatorParameters[2]);
    waveShapeToggles[2]->setExclusiveToggles(true);
    waveShapeToggles[2]->setConfig(ofJson({{"type", "radio"}}));
    //*/
    volume3.set("Volume 3", 0.1, 0, 1);
    panel3->add(volume3);
    volume3.addListener(this, &ofApp::setVolume3);

    frequency3.set("Frequency 3", 1200, 0, 2000);
    panel3->add(frequency3);
    frequency3.addListener(this, &ofApp::setFrequency3);

    pan3.set("Pan 3", 0, 0, 1);
    panel3->add(pan3);
    pan3.addListener(this, &ofApp::setPan3);
    
    /*
     * adding listeners
     */
    //showHeaders.addListener(this, &ofApp::toggleGroupHeader);
    /*
    for (int i = 0; i < 3; i++) {
        waveShapeToggles[i]->getActiveToggleIndex().addListener(this, &ofApp::setWaveShape1);
        //waveShapeToggles[i]->setActiveToggle(3);
    }
    */
    waveShapeToggles[0]->getActiveToggleIndex().addListener(this, &ofApp::setWaveShape1);
    waveShapeToggles[1]->getActiveToggleIndex().addListener(this, &ofApp::setWaveShape2);
    waveShapeToggles[2]->getActiveToggleIndex().addListener(this, &ofApp::setWaveShape3);


    
    
    /*
    waveShapeToggles1->getActiveToggleIndex().addListener(this, &ofApp::setWaveShape1);
    waveShapeToggles1->setActiveToggle(9);
    */
    // midi ------------------------------------------------------------
    // print input ports to console
    midiIn.listInPorts();
    // open port by number (you may need to change this)
    midiIn.openPort(0);
    //midiIn.openPort("IAC Pure Data In");    // by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    // add ofApp as a listener
    midiIn.addListener(this);
    // print received messages to the console
    midiIn.setVerbose(false);
    midiIn.listInPorts();
    // -----------------------------------
    ofBackground(34, 34, 34);
    int bufferSize = 512;
    sampleRate = 44100;
    phase = 0.0;
    phaseAdder = 0.1f;
    phaseAdderTarget = 0.0f;
    volume = 0.1f;
    bNoise = false;
    lAudio.assign(bufferSize, 0.0); // vector <float> lAudio;
    rAudio.assign(bufferSize, 0.0);
    // save oscillator wave
    osci1 = new Oscillator(bufferSize, sampleRate,  400.0, volume1, pan1);
    osci2 = new Oscillator(bufferSize, sampleRate,  800.0, volume2, pan2);
    osci3 = new Oscillator(bufferSize, sampleRate, 1200.0, volume3, pan3);

    soundStream.printDeviceList();
    ofSoundStreamSettings settings;
    // if you want to set the device id to be different than the default:
    auto devices = soundStream.getDeviceList();
    settings.setOutDevice(devices[1]);
    // you can also get devices for an specific api:
    //
    //    auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
    //    settings.setOutDevice(devices[0]);

    // or get the default device for an specific api:
    //
    // settings.api = ofSoundDevice::Api::PULSE;

    // or by name:
    //
    //    auto devices = soundStream.getMatchingDevices("default");
    //    if(!devices.empty()){
    //        settings.setOutDevice(devices[0]);
    //    }

#ifdef TARGET_LINUX
    // Latest linux versions default to the HDMI output
    // this usually fixes that. Also check the list of available
    // devices if sound doesn't work
    auto devices = soundStream.getMatchingDevices("default");
    if (!devices.empty())
    {
        settings.setOutDevice(devices[0]);
    }
#endif

    //ofLog(OF_LOG_NOTICE, "--------------------------------------");
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);

    countSoundFrame = 0;
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
    /*
    ofFile::removeFile("text.txt");
    myTextFile.open("text.txt",ofFile::Append);
    myTextFile << "countSoundFrame" << "," << "phaseAdder" << "," << "phaseAdderTarget" << endl;
    */

//    btnWaveSin1.setup("btnWaveSin1");
}

//--------------------------------------------------------------
void ofApp::update()
{
    MidiControllers MidiCCs = MidiControllers::OSCI1_FREQ;
    float heightPct = 0.0;
    float tmpFreq = 0.0;
    int i = midiMessages.size();
    while (!midiMessages.empty())
    {
        // ofLog(OF_LOG_NOTICE, "i = %d",i);
        ofxMidiMessage message = midiMessages.front();
        // MIDI コントロールのそれぞれのボタンに対応した処理
        switch (message.control)
        {
        // ------------------ controler 1
        case static_cast<int>(MidiControllers::OSCI1_FREQ) : // frequency control
            osci1->setFrequency(2000.0f * (float)message.value / 127.0);
            break;
        case static_cast<int>(MidiControllers::OSCI1_VOL): // volume
            osci1->setVolume((float)message.value / 127.0);
            break;
        case static_cast<int>(MidiControllers::OSCI1_PAN):  // pan
            osci1->setPan((float)message.value / 127.0);
            break;
        case static_cast<int>(MidiControllers::OSCI1_WAVE_SIN):  // sin osci
            //osci1->setWaveShape(0);
            osci1->setWaveShape(Oscillator::WaveShape::SIN);
            break;
        case static_cast<int>(MidiControllers::OSCI1_WAVE_RECT):  // rectangle wave
            //osci1->setWaveShape(1);
            osci1->setWaveShape(Oscillator::WaveShape::RECT);
            break;
        case static_cast<int>(MidiControllers::OSCI1_WAVE_SAW):  // saw wave
            //osci1->setWaveShape(2);
            osci1->setWaveShape(Oscillator::WaveShape::SAW);
            break;
        // ------------------ controler 2
        case static_cast<int>(MidiControllers::OSCI2_FREQ) : // frequency control
            osci2->setFrequency(2000.0f * (float)message.value / 127.0);
            break;
        case static_cast<int>(MidiControllers::OSCI2_VOL): // volume
            osci2->setVolume((float)message.value / 127.0);
            break;
        case static_cast<int>(MidiControllers::OSCI2_PAN):  // pan
            osci2->setPan((float)message.value / 127.0);
            break;
        case static_cast<int>(MidiControllers::OSCI2_WAVE_SIN):  // sin osci
            //osci2->setWaveShape(0);
            osci2->setWaveShape(Oscillator::WaveShape::SIN);
            break;
        case static_cast<int>(MidiControllers::OSCI2_WAVE_RECT):  // rectangle wave
            //osci2->setWaveShape(1);
            osci2->setWaveShape(Oscillator::WaveShape::RECT);
            break;
        case static_cast<int>(MidiControllers::OSCI2_WAVE_SAW):  // saw wave
            //osci2->setWaveShape(2);
            osci2->setWaveShape(Oscillator::WaveShape::SAW);
            break;
        // ------------------ controler 3
        /*
        case 2:
            osci3->setFrequency(2000.0f * (float)message.value / 127.0);
            break;
        case 18: // volume
            osci3->setVolume((float)message.value / 127.0);
            break;
        case 19:  // pan
            osci3->setPan((float)message.value / 127.0);
            break;
        case 34:  // sin wave
            osci3->setWaveShape(0);
            break;
        case 50:  // rectangle wave
            osci3->setWaveShape(1);
            break;
        case 66:  // saw wave
            osci3->setWaveShape(2);
            break;
        */
        // ------------------ controler all
        case 23:
            volume =  (float)message.value / 127.0;
            break;
        default:
            break;
        }
        // MIDIメッセージを削除
        midiMessages.pop();
        i = i - 1;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // width = 900
    float leftScale = 1 - pan;
    float rightScale = pan;
    ofSetColor(225);
    // ofDrawBitmapString("string",x,y);
    ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
    ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 32+12);

    ofNoFill();

    // -------------------------------------------------
    // draw Oscillator 1
    // -------------------------------------------------
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, gY, 0); // Change origin of the coordinates.
    ofSetColor(146,183,254);
    ofSetBackgroundColor(146,183,254);
//    btnWaveSin1.setColor(146,183,254);
    //btnWaveSin1.setPosition(32,150);
    //btnWaveSin1.draw();

    ofSetColor(225);
    ofDrawBitmapString("Oscillator 1", 4, 18);
    ofDrawBitmapString(osci1->shapeName + " wave (" + ofToString(osci1->frequency) + "hz) : CC #0", 4, 2*18);
    // NAMEOF_ENUM(MidiControllers::OSCI1_VOL)
    //ofDrawBitmapString("volume (" + ofToString(osci1->volume) + ") : CC #16", 4, 3*18);
    //ofDrawBitmapString("volume (" + ofToString(osci1->volume) + ") : CC #" + NAMEOF_ENUM(MidiControllers::OSCI1_VOL), 4, 3*18);
    ofDrawBitmapString("pan (" + ofToString(osci1->pan) + ") : CC #1", 4, 4*18);
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, gWidth, gHeight); // draw frame of graph
    //--------------- draw grpah
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci1->getLGraph(i);
        //ofMap: Given a value and an input range, map the value to an output range.
        float x = ofMap(i, 0, 400, 0, gWidth, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    //--------------- draw grpah
    ofSetColor(0, 255, 255);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci1->getRGraph(i);
        float x = ofMap(i, 0, 400, 0, gWidth, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    ofPopMatrix(); // undo coordinates
    ofPopStyle();

    // -------------------------------------------------
    // draw Oscillator 2
    // -------------------------------------------------
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32+gWidth, gY, 0);
    ofSetColor(225);
    ofDrawBitmapString("Oscillator 2", 4, 18);
    ofDrawBitmapString(osci2->shapeName + " wave (" + ofToString(osci2->frequency) + "hz) : CC #2", 4, 2*18);
    ofDrawBitmapString("volume (" + ofToString(osci2->volume) + ") : CC #18", 4, 3*18);
    ofDrawBitmapString("pan (" + ofToString(osci2->pan) + ") : CC #19", 4, 4*18);
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, gWidth, gHeight);
    //--------------- draw grpah
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci2->getLGraph(i);
        float x = ofMap(i, 0, 400, 0, gWidth, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    //--------------- draw grpah
    ofSetColor(0, 255, 255);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci2->getRGraph(i);
        float x = ofMap(i, 0, 400, 0, gWidth, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();

    // -------------------------------------------------
    // draw Oscillator 3
    // -------------------------------------------------
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32+2*gWidth, gY, 0);
    ofSetColor(225);
    ofDrawBitmapString("Oscillator 3", 4, 18);
    ofDrawBitmapString(osci3->shapeName + " wave (" + ofToString(osci3->frequency) + "hz) : CC #2", 4, 2*18);
    ofDrawBitmapString("volume (" + ofToString(osci3->volume) + ") : CC #18", 4, 3*18);
    ofDrawBitmapString("pan (" + ofToString(osci3->pan) + ") : CC #19", 4, 4*18);
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, gWidth, gHeight);
    //--------------- draw grpah
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci3->getLGraph(i);
        float x = ofMap(i, 0, 400, 0, gWidth, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    //--------------- draw grpah
    ofSetColor(0, 255, 255);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci3->getRGraph(i);
        float x = ofMap(i, 0, 400, 0, gWidth, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();

    // -------------------------------------------------
    // draw sum wave:
    // -------------------------------------------------
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, gY+2*gHeight+50, 0);
    ofSetColor(225);
    ofDrawBitmapString("Sum", 4, 18);
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, gWidth * 3, gHeight);
    //--------------- draw grpah
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    ofBeginShape();
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci1->getLGraph(i) + osci2->getLGraph(i) + osci3->getLGraph(i);
        float x = ofMap(i, 0, 400, 0, 450, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    ofSetColor(0, 255, 255);
    ofSetLineWidth(3);
    ofBeginShape();
    //--------------- draw grpah
    for (unsigned int i = 0; i < 400; i ++)
    {
        float sample = osci1->getRGraph(i) + osci2->getRGraph(i) + osci3->getRGraph(i);
        float x = ofMap(i, 0, 400, 0, 450, true);
        ofVertex(x, gHeightCenter - sample * 180.0f);
    }
    ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();

    ofSetColor(225);
    string reportString = "volume: (" + ofToString(volume, 2) + ") modify with -/+ keys\npan: (" + ofToString(pan, 2) + ") modify with mouse x\nsynthesis: ";
    ofDrawBitmapString(reportString, gWidth*3-250, gY+2*gHeight+70);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == '-' || key == '_')
    {
        volume -= 0.05;
        volume = MAX(volume, 0);
    }
    else if (key == '+' || key == '=')
    {
        volume += 0.05;
        volume = MIN(volume, 1);
    }

    if (key == 's')
    {
        soundStream.start();
    }

    if (key == 'e')
    {
        soundStream.stop();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------

void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    int width = ofGetWidth();
    pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
//    bNoise = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
//    bNoise = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer)
{

    // phaseAdder = 0.1f;
    // phaseAdder で phase の値を0-TWO_PI までインクリメントする
    // phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
    // なぜいきなりphaseAdderをphaseAdderTargetにしないか不明だが、
    // 時間が経過すると phaseAdder と phaseAdderTarget ほぼ等しくなる。
    osci1->phaseAdder = 0.95f * osci1->phaseAdder + 0.05f * osci1->phaseAdderTarget;
    osci2->phaseAdder = 0.95f * osci2->phaseAdder + 0.05f * osci2->phaseAdderTarget;
    osci3->phaseAdder = 0.95f * osci3->phaseAdder + 0.05f * osci3->phaseAdderTarget;
    // osci2->phaseAdder = 0.95f * osci2->phaseAdder + 0.05f * osci2->phaseAdderTarget;
    // osci1->phase = 0.0;
    // osci2->phase = 0.0;

    //myTextFile << "phaseAdder";
    // myTextFile << countSoundFrame << "," << ofToString(osci1->phaseAdder) << "," << ofToString(osci1->phaseAdderTarget)<< endl;
    // countSoundFrame++;
    // buffer.getNumFrames() == 512
    for (size_t i = 0; i < buffer.getNumFrames(); i++)
    {
//            float sample = 0.0;
        float lSample = 0.0;
        float rSample = 0.0;

        ///*
        //sample = osci1->getLSample() + osci2->getLSample();
        lSample = osci1->getLSample() + osci2->getLSample() + osci3->getLSample();
        rSample = osci1->getRSample() + osci2->getRSample() + osci3->getRSample();
        //sample = osci1->getSample();
        //*/
        //sample = osci1->LTimeseries[i];

        // S(t)=(t mod(サンプリング周波数/振動数))×((振幅×2)/(サンプリング周波数/振動数))−振幅
        //float sample = fmod(phase , (float)sampleRate / targetFrequency) - 1.0;
        //phaseAdderTarget = (targetFrequency / (float)sampleRate) * TWO_PI;

        // sampleRate
        /*
        lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
        rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;
        */

        // buffer.getNumChannels() : 2
        lAudio[i] = buffer[i * buffer.getNumChannels()] = lSample * volume;
        rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = rSample * volume;
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
void ofApp::exit()
{
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
    delete osci1;
    delete osci2;
    vector<float>().swap(lAudio);
    vector<float>().swap(rAudio);

//    delete wave_sum;
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg)
{
    midiMessages.push(msg);
}

void ofApp::setWaveShape1(int& index){
    switch(index){
        case 0: osci1->setWaveShape(Oscillator::WaveShape::SIN);  break;
        case 1: osci1->setWaveShape(Oscillator::WaveShape::RECT); break;
        case 2: osci1->setWaveShape(Oscillator::WaveShape::SAW);  break;
        default: break;
    }
}

void ofApp::setWaveShape2(int& index){
    switch(index){
        case 0: osci2->setWaveShape(Oscillator::WaveShape::SIN);  break;
        case 1: osci2->setWaveShape(Oscillator::WaveShape::RECT); break;
        case 2: osci2->setWaveShape(Oscillator::WaveShape::SAW);  break;
        default: break;
    }
}

void ofApp::setWaveShape3(int& index){
    switch(index){
        case 0: osci3->setWaveShape(Oscillator::WaveShape::SIN);  break;
        case 1: osci3->setWaveShape(Oscillator::WaveShape::RECT); break;
        case 2: osci3->setWaveShape(Oscillator::WaveShape::SAW);  break;
        default: break;
    }
}

void ofApp::setVolume1(float& volume){
    osci1->setVolume(volume);
}

void ofApp::setVolume2(float& volume){
    osci2->setVolume(volume);
}
void ofApp::setVolume3(float& volume){
    osci3->setVolume(volume);
}

void ofApp::setFrequency1(float& frequency){
    osci1->setFrequency(frequency);
}

void ofApp::setFrequency2(float& frequency){
    osci2->setFrequency(frequency);
}

void ofApp::setFrequency3(float& frequency){
    osci3->setFrequency(frequency);
}

void ofApp::setPan1(float& pan){
    osci1->setPan(pan);
}

void ofApp::setPan2(float& pan){
    osci2->setPan(pan);
}

void ofApp::setPan3(float& pan){
    osci3->setPan(pan);
}
