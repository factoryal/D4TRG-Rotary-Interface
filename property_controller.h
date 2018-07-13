#pragma once

#include "Arduino.h"

class Property {
protected:
	char symbol;
	int8_t val;
	int8_t min_val;
	int8_t max_val;

public:
	

	// this will update the peripheral on demand.
	virtual void update() {

	}
};

class SpeakerVolume : public Property {
public:
	SpeakerVolume(char symbol, int8_t min, int8_t max) {
		this->symbol = symbol;
		min_val = min;
		max_val = max;
	}
};

class HeadsetVolume : public Property {
public:
	HeadsetVolume(char symbol, int8_t min, int8_t max) {
		this->symbol = symbol;
		min_val = min;
		max_val = max;
	}
};

class LEDBrightness : public Property {
public:
	LEDBrightness(char symbol, int8_t min, int8_t max) {
		this->symbol = symbol;
		min_val = min;
		max_val = max;
	}
};

class Properties {
private:
	Property * p[3];
	SegmentUpdater* h; //SegmentUpdater handler

public:
	Properties(SegmentUpdater* handler) {
		h = handler;

		p[0] = new SpeakerVolume('S', 0, 100);
		p[1] = new HeadsetVolume('H', 0, 100);
		p[2] = new LEDBrightness('L', 0, 8);
	}



};