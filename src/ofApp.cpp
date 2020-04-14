#include "ofApp.h"
#include "wave.h"

//--------------------------------------------------------------
void ofApp::setup()
{
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
	wave1 = new Wave(bufferSize, sampleRate, 500.0, 0, 16);
	wave2 = new Wave(bufferSize, sampleRate, 500.0, 1, 17);
	wave_sum = new Wave(bufferSize, sampleRate, 0.0, 8, 23);
	/*
	wave1->assign(bufferSize, 0.0);
	wave2->assign(bufferSize, 0.0);
	wave_sum.assign(bufferSize, 0.0);
	*/
	
	soundStream.printDeviceList();
	ofSoundStreamSettings settings;
	// if you want to set the device id to be different than the default:
	//
	auto devices = soundStream.getDeviceList();
	// 複数出力装置から音声を出力する
	// settings.setOutDevice(devices[4]);
	settings.setOutDevice(devices[3]);

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

	ofLog(OF_LOG_NOTICE, "--------------------------------------");
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

	// on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
	// use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
}

//--------------------------------------------------------------
void ofApp::update()
{
	float heightPct = 0.0;
	float tmpFreq = 0.0;
	for (unsigned int i = 0; i < midiMessages.size(); ++i)
	{
		ofxMidiMessage &message = midiMessages[i];
		// MIDI コントロールのそれぞれのボタンに対応した処理
		switch (message.control)
		{
		// ------------------ controler 1
		case 0: // frequency
			wave1->setFrequency(2000.0f * (float)message.value / 127.0);
			break;
		case 16: // volume
			wave1->volume = (float)message.value / 127.0;
			break;
		case 17:  // pan
			wave1->pan =  (float)message.value / 127.0;
			break;
		case 32:
			wave1->shape = 0;
			break;
		case 48:
			wave1->shape = 1;
			break;
		// ------------------ controler 2
		case 2:
			wave2->setFrequency(2000.0f * (float)message.value / 127.0);
			break;
		// ------------------ controler 2
		case 18:
			wave2->volume = (float)message.value / 127.0;
			break;
		case 19:  // pan
			wave2->pan =  (float)message.value / 127.0;
			break;
		// ------------------ controler all
		case 23:
			volume =  (float)message.value / 127.0;
			break;
		default:
			break;
		}
	}
	// midiCCs.out(1).meter_output() は 0.0 〜 1.0 の小数
	//	drawMeter( midiCCs.out(1).meter_output(), 0.05f, 1.0f, xBase, 30, 20, 200);
	//int width = ofGetWidth();
	// pan = (float)x / (float)width;
	// float height = (float)ofGetHeight();
	// float heightPct = ((height-y) / height);

	/*
	ofLog(OF_LOG_NOTICE, "pan: %f", pan);
	ofLog(OF_LOG_NOTICE, "heightPct: %f", heightPct);
	ofLog(OF_LOG_NOTICE, "targetFrequency: %f", tmpFreq);
	ofLog(OF_LOG_NOTICE, "pan: %f", pan); 
	ofLog(OF_LOG_NOTICE, "phaseAdderTarget: %f", phaseAdderTarget); 
	*/
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofSetColor(225);
	// ofDrawBitmapString("string",x,y);
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);

	ofNoFill();

	// -------------------------------------------------
	// draw the left channel 1:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 150, 0); // 座標の原点を変更する

	// グラフのスタイル 
	ofSetColor(225);
	ofDrawBitmapString("Channel 1 Left", 4, 18);
	ofDrawBitmapString("sine wave (" + ofToString(wave1->frequency) + "hz) modify with CC #0", 4, 2*18);
	ofDrawBitmapString("volume (" + ofToString(wave1->volume) + ") modify with CC #16", 4, 3*18);
	ofDrawBitmapString("pan (" + ofToString(wave1->pan) + ") modify with CC #1", 4, 4*18);

	//ofDrawBitmapString(ofToString(targetfrequency, 2) + "hz) modify with mouse y";

	ofSetLineWidth(1);
	// グラフの枠を描画する
	ofDrawRectangle(0, 0, 450, 200);

	// 時系列のスタイル 
	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	float leftScale = 1 - pan;
	float rightScale = pan;

	for (unsigned int i = 0; i < wave1->timeseries.size(); i+=2)
	{
		float sample = wave1->timeseries[i] 
		             * wave1->volume
					 * wave1->leftScale;
		//iの範囲0-lAudio.size()から別の範囲0-900へ変換する
		float x = ofMap(i, 0, wave1->timeseries.size(), 0, 450, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}

	ofEndShape(false);

	// 座標系をもとに戻す
	ofPopMatrix();
	ofPopStyle();

	// -------------------------------------------------
	// draw the left channel 2:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32+450, 150, 0); // 座標の原点を変更する

	// グラフのスタイル 
	ofSetColor(225);
	ofDrawBitmapString("Channel 2 Left", 4, 18);
	ofDrawBitmapString("sine wave (" + ofToString(wave2->frequency) + "hz) modify with CC #2", 4, 2*18);
	ofDrawBitmapString("volume (" + ofToString(wave2->volume) + ") modify with CC #18", 4, 3*18);
	ofDrawBitmapString("pan (" + ofToString(wave2->pan) + ") modify with CC #19", 4, 4*18);

	ofSetLineWidth(1);
	// グラフの枠を描画する
	ofDrawRectangle(0, 0, 450, 200);
	
	// 時系列のスタイル 
	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);
	ofBeginShape();
	// グラフを描画する
	for (unsigned int i = 0; i < wave2->timeseries.size(); i+=2)
	{
		float sample = wave2->timeseries[i]
		             * wave2->volume
					 * wave2->leftScale;
		//iの範囲0-lAudio.size()から別の範囲に変換する
		float x = ofMap(i, 0, wave2->timeseries.size(), 0, 450, true);
		// 点を描画する
		ofVertex(x, 100 - sample * 180.0f);
	}
	ofEndShape(false);

	// 座標系をもとに戻す
	ofPopMatrix();
	ofPopStyle();


	// -------------------------------------------------
	// draw sum wave:
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
	for (unsigned int i = 0; i < wave1->timeseries.size(); i++)
	{	
		float sample = wave1->timeseries[i] * wave1->volume * wave1->leftScale
			 		 + wave2->timeseries[i] * wave2->volume * wave2->leftScale;
		//iの範囲0-lAudio.size()から別の範囲に変換する
		float x = ofMap(i, 0, wave1->timeseries.size(), 0, 450, true);
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

	// -------------------------------------------------
	// draw the right channel:
	/*
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 350, 0);

	ofSetColor(225);
	ofDrawBitmapString("Right Channel", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 900, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < rAudio.size(); i++)
	{
		float x = ofMap(i, 0, rAudio.size(), 0, 900, true);
		ofVertex(x, 100 - rAudio[i] * 180.0f);
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
	*/
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
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

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
		// 	phaseAdder = 0.1f;
		// phaseAdder で phase の値を0-TWO_PI までインクリメントする
		// phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		wave1->phaseAdder = 0.95f * wave1->phaseAdder + 0.05f * wave1->phaseAdderTarget;
		wave2->phaseAdder = 0.95f * wave2->phaseAdder + 0.05f * wave2->phaseAdderTarget;
		// buffer.getNumFrames() == 512
		for (size_t i = 0; i < buffer.getNumFrames(); i++)
		{
			float sample = 0.0;
			wave1->timeseries[i] = wave1->getSample();
			///*
			wave2->timeseries[i] = wave2->getSample();
			sample = wave1->timeseries[i] + wave2->timeseries[i];
			//*/
			//sample = wave1->timeseries[i];

			///sample = sin(phase);
			//sample += sin(2*phase);
			//square wave  
			//float sample = sin(phase)>0?1:-1;
			//saw wave  
			//float sample = phase / TWO_PI;
			// S(t)=(t mod(サンプリング周波数/振動数))×((振幅×2)/(サンプリング周波数/振動数))−振幅  
			//float sample = fmod(phase , (float)sampleRate / targetFrequency) - 1.0;  
       		//phaseAdderTarget = (targetFrequency / (float)sampleRate) * TWO_PI;

			// sampleRate

			lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;

			/*
			lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;
			*/
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
	delete wave1;
	delete wave2;
	delete wave_sum;
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg)
{
	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while (midiMessages.size() > maxMessages)
	{
		midiMessages.erase(midiMessages.begin());
	}
}