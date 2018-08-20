#pragma once

#include "Arduino.h"

#include "M62429.h"

enum AUDIO_DEVICE {
	SPEAKER = 0, HEADSET
};

class VolumeController {
private:
	

	PROGMEM byte volmap[34] = {
		0, 29, 33, 37, 41,
		45, 48, 51, 54, 57,
		60, 63, 66, 69, 72,
		74, 76, 78, 80, 82,
		84, 86, 88, 90, 91,
		92, 93, 94, 95, 96,
		97, 98, 99, 100
	};

	M62429* m62429[2];

public:

	VolumeController(uint8_t data1, uint8_t clk1, uint8_t data2, uint8_t clk2) {
		m62429[SPEAKER] = new M62429(clk1, data1);
		m62429[HEADSET] = new M62429(clk2, data2);
	}

	// set volume of audio output device
	// parameter d is device id.
	// parameter v must be in range 0~33.
	void setVolume(uint8_t d, uint8_t v) {
		m62429[d]->setBoth(volmap[v]);
	}


};

