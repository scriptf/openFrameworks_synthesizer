enum class MidiControllers : int
{
    OSCI1_FREQ = 0,
    OSCI1_VOL = 16,
    OSCI1_PAN = 17,
    OSCI1_WAVE_SIN = 32,
    OSCI1_WAVE_RECT = 48,
    OSCI1_WAVE_SAW = 64,
    OSCI2_FREQ = 2,
    OSCI2_VOL = 18,
    OSCI2_PAN = 19,
    OSCI2_WAVE_SIN = 34,
    OSCI2_WAVE_RECT = 50,
    OSCI2_WAVE_SAW = 66,
    OSCI3_FREQ = 0,
    OSCI3_VOL = 16,
    OSCI3_PAN = 17,
    OSCI3_WAVE_SIN = 32,
    OSCI3_WAVE_RECT = 48,
    OSCI3_WAVE_SAW = 64,
};


/*
case 2:
				osci2->setFrequency(2000.0f * (float)message.value / 127.0);
				break;
			case 18: // volume
				osci2->setVolume((float)message.value / 127.0);
				break;
			case 19:  // pan
				osci2->setPan((float)message.value / 127.0);
				break;
			case 34:  // sin wave
				osci2->setWaveShape(0);
				break;
			case 50:  // rectangle wave
				osci2->setWaveShape(1);
				break;
			case 66:  // saw wave
				osci2->setWaveShape(2);
				break;
			
*/