#include "ofApp.h"
#include "oscillator.h"
#include "midi_controllers.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	pan = 0.0;
	leftScale = 1.0 - pan;
    rightScale = pan;

	// midi ------------------------------------------------------------
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	// print input ports to console
	midiIn.listInPorts();

	// open port by number (you may need to change this)
	midiIn.openPort(0);
	//midiIn.openPort("IAC Pure Data In");	// by name
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

	// 波形を保存する
	//vector<vector<float>> waves;
	osci1 = new Oscillator(bufferSize, sampleRate,  400.0, 0, 16);
	osci2 = new Oscillator(bufferSize, sampleRate,  800.0, 1, 17);
	osci3 = new Oscillator(bufferSize, sampleRate, 1200.0, 1, 17);
	//wave_sum = new Wave(bufferSize, sampleRate, 0.0, 8, 23);

	soundStream.printDeviceList();
	ofSoundStreamSettings settings;
	// if you want to set the device id to be different than the default:
	//
	auto devices = soundStream.getDeviceList();
	// 複数出力装置から音声を出力する
	// settings.setOutDevice(devices[4]);
	//settings.setOutDevice(devices[4]);
	settings.setOutDevice(devices[1]);

	// you can also get devices for an specific api:
	//
	//	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
	//	settings.setOutDevice(devices[0]);

	// or get the default device for an specific api:
	//
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name:
	//
	//	auto devices = soundStream.getMatchingDevices("default");
	//	if(!devices.empty()){
	//		settings.setOutDevice(devices[0]);
	//	}

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
	ofFile::removeFile("text.txt");
	myTextFile.open("text.txt",ofFile::Append);
	myTextFile << "countSoundFrame" << "," << "phaseAdder" << "," << "phaseAdderTarget" << endl;

	// on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
	// use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
}

//--------------------------------------------------------------
void ofApp::update()
{
	MidiControllers MidiCCs = MidiControllers::OSCI1_FREQ;
	float heightPct = 0.0;
	float tmpFreq = 0.0;
	int i = midiMessages.size();
    while (!midiMessages.empty()) {
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
				osci2->setWaveShape(0);
				break;
			case static_cast<int>(MidiControllers::OSCI2_WAVE_RECT):  // rectangle wave
				osci2->setWaveShape(1);
				break;
			case static_cast<int>(MidiControllers::OSCI2_WAVE_SAW):  // saw wave
				osci2->setWaveShape(2);
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
	int gWidth = 300; // Width of graph 
	float leftScale = 1 - pan;
	float rightScale = pan;
	ofSetColor(225);
	// ofDrawBitmapString("string",x,y);
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);

	ofNoFill();

	// -------------------------------------------------
	// draw Oscillator 1
	// -------------------------------------------------
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 150, 0); // 座標の原点を変更する

	// グラフのスタイル 
	ofSetColor(225);
	ofDrawBitmapString("Oscillator 1", 4, 18);
	ofDrawBitmapString("sine wave (" + ofToString(osci1->frequency) + "hz) modify with CC #0", 4, 2*18);
	ofDrawBitmapString("volume (" + ofToString(osci1->volume) + ") modify with CC #16", 4, 3*18);
	ofDrawBitmapString("pan (" + ofToString(osci1->pan) + ") modify with CC #1", 4, 4*18);

	//ofDrawBitmapString(ofToString(targetfrequency, 2) + "hz) modify with mouse y";

	ofSetLineWidth(1);
	// グラフの枠を描画する
	ofDrawRectangle(0, 0, gWidth, 200);

	// 時系列のスタイル 
	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci1->LTimeseries[i];
		float sample = osci1->getLGraph(i);		
		//iの範囲0から別の範囲0-900へ変換する
		float x = ofMap(i, 0, 400, 0, gWidth, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 時系列のスタイル 
	ofSetColor(0, 255, 255);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci1->LTimeseries[i];
		float sample = osci1->getRGraph(i);		
		//iの範囲0から別の範囲0-900へ変換する
		float x = ofMap(i, 0, 400, 0, gWidth, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 座標系をもとに戻す
	ofPopMatrix();
	ofPopStyle();

	// -------------------------------------------------
	// draw Oscillator 2
	// -------------------------------------------------
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32+gWidth, 150, 0); // 座標の原点を変更する

	// グラフのスタイル s
	ofSetColor(225);
	ofDrawBitmapString("Oscillator 2", 4, 18);
	ofDrawBitmapString("sine wave (" + ofToString(osci2->frequency) + "hz) modify with CC #2", 4, 2*18);
	ofDrawBitmapString("volume (" + ofToString(osci2->volume) + ") modify with CC #18", 4, 3*18);
	ofDrawBitmapString("pan (" + ofToString(osci2->pan) + ") modify with CC #19", 4, 4*18);

	ofSetLineWidth(1);
	// グラフの枠を描画する
	ofDrawRectangle(0, 0, gWidth, 200);
	
	// 時系列のスタイル 
	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci2->LTimeseries[i];
		float sample = osci2->getLGraph(i);		
		//iの範囲0から別の範囲0-gWidthへ変換する
		float x = ofMap(i, 0, 400, 0, gWidth, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 時系列のスタイル 
	ofSetColor(0, 255, 255);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci1->LTimeseries[i];
		float sample = osci2->getRGraph(i);		
		//iの範囲0から別の範囲0-900へ変換する
		float x = ofMap(i, 0, 400, 0, gWidth, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 座標系をもとに戻す
	ofPopMatrix();
	ofPopStyle();

	// -------------------------------------------------
	// draw Oscillator 3
	// -------------------------------------------------
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32+2*gWidth, 150, 0); // 座標の原点を変更する

	// グラフのスタイル 
	ofSetColor(225);
	ofDrawBitmapString("Oscillator 3", 4, 18);
	ofDrawBitmapString("sine wave (" + ofToString(osci3->frequency) + "hz) modify with CC #2", 4, 2*18);
	ofDrawBitmapString("volume (" + ofToString(osci3->volume) + ") modify with CC #18", 4, 3*18);
	ofDrawBitmapString("pan (" + ofToString(osci3->pan) + ") modify with CC #19", 4, 4*18);

	ofSetLineWidth(1);
	// グラフの枠を描画する
	ofDrawRectangle(0, 0, gWidth, 200);
	
	// 時系列のスタイル 
	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci3->LTimeseries[i];
		float sample = osci3->getLGraph(i);		
		//iの範囲0から別の範囲0-gWidthへ変換する
		float x = ofMap(i, 0, 400, 0, gWidth, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 時系列のスタイル 
	ofSetColor(0, 255, 255);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci1->LTimeseries[i];
		float sample = osci3->getRGraph(i);		
		//iの範囲0から別の範囲0-900へ変換する
		float x = ofMap(i, 0, 400, 0, gWidth, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 座標系をもとに戻す
	ofPopMatrix();
	ofPopStyle();


	// -------------------------------------------------
	// draw sum wave:
	// -------------------------------------------------
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 350, 0);

	ofSetColor(225);
	ofDrawBitmapString("Sum", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 900, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci1->LTimeseries[i] + osci2->LTimeseries[i];
		float sample = osci1->getLGraph(i) + osci2->getLGraph(i) + osci3->getLGraph(i);		
		//iの範囲0から別の範囲0-900へ変換する
		float x = ofMap(i, 0, 400, 0, 450, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	ofSetColor(0, 255, 255);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < 400; i ++)
	{
		//float sample = osci1->LTimeseries[i];
		float sample = osci1->getRGraph(i) + osci2->getRGraph(i) + osci3->getRGraph(i);		
		//iの範囲0から別の範囲0-900へ変換する
		float x = ofMap(i, 0, 400, 0, 450, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);



	ofPopMatrix();
	ofPopStyle();

	ofSetColor(225);
	string reportString = "volume: (" + ofToString(volume, 2) + ") modify with -/+ keys\npan: (" + ofToString(pan, 2) + ") modify with mouse x\nsynthesis: ";
	if (!bNoise)
	{
		reportString += "sine wave (" + ofToString(targetFrequency, 2) + "hz) modify with mouse y";
	}
	else
	{
		reportString += "noise";
	}
	ofDrawBitmapString(reportString, 32, 579);

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
	bNoise = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	bNoise = false;
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
	if (bNoise == true)
	{
		// ---------------------- noise --------------
		for (size_t i = 0; i < buffer.getNumFrames(); i++)
		{
			lAudio[i] = buffer[i * buffer.getNumChannels()] = ofRandom(0, 1) * volume * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = ofRandom(0, 1) * volume * rightScale;
		}
	}
	else
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
//			float sample = 0.0;
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

//	delete wave_sum;
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg)
{
    midiMessages.push(msg);
}