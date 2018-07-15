#pragma once

#include "Arduino.h"

class Property {
protected:
	char symbol;
	int8_t val;
	int8_t min_val;
	int8_t max_val;
	uint8_t step_unit;
	bool zero_digit;

public:
	
	// this will update the peripheral on demand.
	virtual bool update(int8_t amount) = 0;

	int8_t getVal() {
		return val;
	}

	int8_t getLevel() {
		return map(val, min_val, max_val, 0, 8);
	}
};

class SpeakerVolume : public Property {
public:
	SpeakerVolume(char symbol, int8_t min, int8_t max, uint8_t step, bool zerodigit) {
		this->symbol = symbol;
		min_val = min;
		max_val = max;
		step_unit = step;
		zero_digit = zerodigit;
	}

	bool update(int8_t amount) {
		if (amount) {
			val += amount;
			if (val < min_val) val = min_val;
			else if (val > max_val) val = max_val;


		}
		return zero_digit;
	}
};

class HeadsetVolume : public Property {
public:
	HeadsetVolume(char symbol, int8_t min, int8_t max, uint8_t step, bool zerodigit) {
		this->symbol = symbol;
		min_val = min;
		max_val = max;
		step_unit = step;
		zero_digit = zerodigit;
	}

	bool update(int8_t amount) {
		if (amount) {
		}
		return zero_digit;
	}
};

class LEDBrightness : public Property {
public:
	LEDBrightness(char symbol, int8_t min, int8_t max, uint8_t step, bool zerodigit) {
		this->symbol = symbol;
		min_val = min;
		max_val = max;
		step_unit = step;
		zero_digit = zerodigit;
	}

	bool update(int8_t amount) {
		if (amount) {
		}
		return zero_digit;
	}
};

class Properties {
private:
	Property * p[3];
	SegmentUpdater* segmentHandler; //SegmentUpdater handler
	Rotary* rotaryHandler;
	uint8_t currentProperty = 0;

public:
	Properties(SegmentUpdater* segment_handler, Rotary* rotary_handler) {
		segmentHandler = segment_handler;
		rotaryHandler = rotary_handler;

		p[0] = new SpeakerVolume('S', 0, 99, 4, true);
		p[1] = new HeadsetVolume('H', 0, 99, 4, true);
		p[2] = new LEDBrightness('L', 0, 8, 1, false);
	}

	void update() {
		char buf[3] = { 0 };
		int8_t val;
		rotaryHandler->update();
		if (p[currentProperty]->update(rotaryHandler->read())) {
			val = p[currentProperty]->getVal();
			if (val <= 9) {
				buf[0] = '0';
				itoa(val, buf + 1, 10);
			}
		}
		else {
			val = p[currentProperty]->getVal();
			itoa(val, buf, 10);
		}
		rotaryHandler->zero();
		
		segmentHandler->setStr(buf);
		segmentHandler->setLevel(p[currentProperty]->getLevel());
	}

};