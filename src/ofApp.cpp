#include "ofApp.h"

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
	phase = 0;
	phaseAdder = 0.0f;
	phaseAdderTarget = 0.0f;
	volume = 0.1f;
	bNoise = false;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	soundStream.printDeviceList();

	ofSoundStreamSettings settings;

	// if you want to set the device id to be different than the default:
	//
	//	auto devices = soundStream.getDeviceList();
	//	settings.setOutDevice(devices[3]);

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
		switch (message.control)
		{
		case 0:
			pan =  (float)message.value / 127.0;
			break;
		case 1:
			heightPct = (float)message.value / 127.0;
			targetFrequency = 2000.0f * heightPct;
			phaseAdderTarget = (targetFrequency / (float)sampleRate) * TWO_PI;
			tmpFreq = targetFrequency; 
			break;
		default:
			break;
		}
	}
	// midiCCs.out(1).meter_output() は 0.0 〜 1.0 の小数
	//	drawMeter( midiCCs.out(1).meter_output(), 0.05f, 1.0f, xBase, 30, 20, 200);
	//int width = ofGetWidth();
	// pan = (float)x / (float)width;
	//float height = (float)ofGetHeight();
	// float heightPct = ((height-y) / height);

	ofLog(OF_LOG_NOTICE, "pan: %f", pan);
	ofLog(OF_LOG_NOTICE, "heightPct: %f", heightPct);
	ofLog(OF_LOG_NOTICE, "targetFrequency: %f", tmpFreq);
	/*
	ofLog(OF_LOG_NOTICE, "pan: %f", pan); 
	ofLog(OF_LOG_NOTICE, "phaseAdderTarget: %f", phaseAdderTarget); 
	*/
}

//--------------------------------------------------------------
void ofApp::draw()
{

	ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);

	ofNoFill();

	// draw the left channel:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 150, 0);

	ofSetColor(225);
	ofDrawBitmapString("Left Channel", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 900, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < lAudio.size(); i++)
	{
		float x = ofMap(i, 0, lAudio.size(), 0, 900, true);
		ofVertex(x, 100 - lAudio[i] * 180.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
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
	/*
	int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
	ofLog(OF_LOG_NOTICE, "pan: %f", pan); 
	ofLog(OF_LOG_NOTICE, "targetFrequency: %f", targetFrequency); 
	ofLog(OF_LOG_NOTICE, "phaseAdderTarget: %f", phaseAdderTarget); 
	*/
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

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI)
	{
		phase -= TWO_PI;
	}

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
		// phaseAddr の上限値は
		phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (size_t i = 0; i < buffer.getNumFrames(); i++)
		{
			phase += phaseAdder;

			//sine wave    
			//float sample = sin(phase);
			//square wave  
			float sample = sin(phase)>0?1:-1;
			//saw wave  
			//float sample = phase / TWO_PI;
			// S(t)=(t mod(サンプリング周波数/振動数))×((振幅×2)/(サンプリング周波数/振動数))−振幅  
			//float sample = fmod(phase , (float)sampleRate / targetFrequency) - 1.0;  
       		//phaseAdderTarget = (targetFrequency / (float)sampleRate) * TWO_PI;

			// sampleRate

			/*
			if(sample < 0.0) {
				sample += TWO_PI;
			}
			sample /= PI;
			sample -= 1.0;
			*/
			ofLog(OF_LOG_NOTICE, "sample: %f", sample); 

			lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;
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