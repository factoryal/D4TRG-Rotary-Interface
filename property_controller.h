#pragma once

#include "Arduino.h"

class Property {
protected:
	char symbol;
	int8_t val = 0;
	int8_t min_val;
	int8_t max_val;
	int8_t min_level;
	int8_t max_level;
	uint8_t step_unit;
	bool zero_digit;

public:

	// this will update the peripheral on demand.
	virtual bool update(int8_t amount) = 0;

	int8_t getVal() {
		return val;
	}

	int8_t getLevel() {
		return map(val, min_val, max_val, min_level, max_level);
	}

	char getSymbol() {
		return symbol;
	}
};

class SpeakerVolume : public Property {
public:
	SpeakerVolume(char symbol, int8_t min_val, int8_t max_val, int8_t min_level, int8_t max_level, uint8_t step, bool zerodigit) {
		this->symbol = symbol;
		this->min_val = min_val;
		this->max_val = max_val;
		this->min_level = min_level;
		this->max_level = max_level;
		step_unit = step;
		zero_digit = zerodigit;
	}

	bool update(int8_t amount) {
		if (amount) {
			val += amount * step_unit;
			if (val < min_val) val = min_val;
			else if (val > max_val) val = max_val;


		}
		return zero_digit;
	}
};

class HeadsetVolume : public Property {
public:
	HeadsetVolume(char symbol, int8_t min_val, int8_t max_val, int8_t min_level, int8_t max_level, uint8_t step, bool zerodigit) {
		this->symbol = symbol;
		this->min_val = min_val;
		this->max_val = max_val;
		this->min_level = min_level;
		this->max_level = max_level;
		step_unit = step;
		zero_digit = zerodigit;
	}

	bool update(int8_t amount) {
		if (amount) {
			val += amount * step_unit;
			if (val < min_val) val = min_val;
			else if (val > max_val) val = max_val;


		}
		return zero_digit;
	}
};

class LEDBrightness : public Property {
public:
	LEDBrightness(char symbol, int8_t min_val, int8_t max_val, int8_t min_level, int8_t max_level, uint8_t step, bool zerodigit) {
		this->symbol = symbol;
		this->min_val = min_val;
		this->max_val = max_val;
		this->min_level = min_level;
		this->max_level = max_level;
		step_unit = step;
		zero_digit = zerodigit;
	}

	bool update(int8_t amount) {
		if (amount) {
			val += amount * step_unit;
			if (val < min_val) val = min_val;
			else if (val > max_val) val = max_val;


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

		p[0] = new SpeakerVolume('S', 0, 99, 1, 8, 3, true);
		p[1] = new HeadsetVolume('H', 0, 99, 1, 8, 3, true);
		p[2] = new LEDBrightness('L', 0, 8, 0, 8, 1, false);
	}

	void update() {
		char buf[3] = { 0 };
		int8_t val;
		int8_t amount;

		static bool isBtnPressed = 0;
		static unsigned long changeTime = 0;
		static bool isZeroSpace = 0;

		if (!digitalRead(PIN_SW)) {
			if (!isBtnPressed) {
				isBtnPressed = 1;
				if (++currentProperty >= 3) currentProperty = 0;
				//
				if (p[currentProperty]->update(amount)) {
					val = p[currentProperty]->getVal();
					if (val <= 9) {
						buf[0] = '0';
						itoa(val, buf + 1, 10);
					}
					else itoa(val, buf, 10);
				}
				else {
					val = p[currentProperty]->getVal();
					if (val <= 9) {
						buf[0] = ' ';
						itoa(val, buf + 1, 10);
					}
					else itoa(val, buf, 10);
				}
				rotaryHandler->zero();
				//Serial.println(buf);

				segmentHandler->setStr(buf);
				segmentHandler->setLevel(p[currentProperty]->getLevel());
				//
			}
		}
		else isBtnPressed = 0;


		rotaryHandler->update();
		amount = rotaryHandler->read();

		if (amount) {
			changeTime = millis();
			isZeroSpace = p[currentProperty]->update(amount);

			val = p[currentProperty]->getVal();
			if (val <= 9) {
				buf[0] = isZeroSpace ? '0' : ' ';
				itoa(val, buf + 1, 10);
			}
			else itoa(val, buf, 10);

			rotaryHandler->zero();
			Serial.println(buf);

			segmentHandler->setStr(buf);
			segmentHandler->setLevel(p[currentProperty]->getLevel());



		}
		else if (millis() - changeTime > 500) {
			buf[1] = p[currentProperty]->getSymbol();
			segmentHandler->setStr(buf);
			segmentHandler->setLevel(p[currentProperty]->getLevel());
		}




	}

};