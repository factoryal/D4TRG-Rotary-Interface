#pragma once

#include "Arduino.h"

class Segment {
protected:
	byte value = 0x00;

public:

	void setRaw(byte value) {
		this->value = value;
	}

	virtual void setValue(byte value) = 0;

	byte getValue() {
		return value;
	}

};

class SevenSegment : public Segment {
private:
	const byte SevenSegmentASCII[37] = {
		0x00, /* (space) */
		0x3F, /* 0 */
		0x06, /* 1 */
		0x5B, /* 2 */
		0x4F, /* 3 */
		0x66, /* 4 */
		0x6D, /* 5 */
		0x7D, /* 6 */
		0x07, /* 7 */
		0x7F, /* 8 */
		0x6F, /* 9 */
		0x77, /* A */
		0x7C, /* B */
		0x39, /* C */
		0x5E, /* D */
		0x79, /* E */
		0x71, /* F */
		0x3D, /* G */
		0x76, /* H */
		0x30, /* I */
		0x1E, /* J */
		0x75, /* K */
		0x38, /* L */
		0x15, /* M */
		0x37, /* N */
		0x3F, /* O */
		0x73, /* P */
		0x6B, /* Q */
		0x33, /* R */
		0x6D, /* S */
		0x78, /* T */
		0x3E, /* U */
		0x3E, /* V */
		0x2A, /* W */
		0x76, /* X */
		0x6E, /* Y */
		0x5B, /* Z */
	};

public:
	void setValue(byte ascii) {
		// lower to upper
		if (ascii >= 'a' && ascii <= 'z') ascii ^= 0x20;

		// mapping
		if (ascii >= '0' && ascii <= '9') ascii -= ('0' - 1);
		else if (ascii >= 'A' && ascii <= 'Z') ascii -= ('A' - 11);
		else ascii = 0;

		value = SevenSegmentASCII[ascii];
	}
};

class LedBar : public Segment {
private:
	const uint8_t BarGauge[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

public:
	void setValue(byte level) {
		value = BarGauge[level];
	}
};

class SegmentUpdater {
private:
	uint32_t oldTime = 0;
	const uint32_t updatePeriod = 4;

	int pinidx[3] = { PIN_S1, PIN_S2, PIN_S3 };
	Segment *segidx[3];
	byte value = 0x00;
	uint8_t currentSegmentIdx = 0;

public:
	SegmentUpdater() {
		segidx[0] = new SevenSegment;
		segidx[1] = new SevenSegment;
		segidx[2] = new LedBar;

		pinMode(PIN_CLK, OUTPUT);
		pinMode(PIN_DATA, OUTPUT);
		pinMode(PIN_LATCH, OUTPUT);
		pinMode(PIN_S1, OUTPUT);
		pinMode(PIN_S2, OUTPUT);
		pinMode(PIN_S3, OUTPUT);
	}

	void update() {
		digitalWrite(PIN_LATCH, 0);
		for (int i = 0; i < 3; i++) {
			digitalWrite(pinidx[i], 0);
		}
		shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, segidx[currentSegmentIdx]->getValue());
		digitalWrite(PIN_LATCH, 1);

		digitalWrite(pinidx[currentSegmentIdx], 1);
		if (++currentSegmentIdx >= 3) currentSegmentIdx = 0;

		oldTime = millis();
	}

	void setLevel(const byte level) {
		segidx[2]->setValue(level);
	}

	void setStr(const char c) {
		segidx[0]->setValue(' ');
		segidx[1]->setValue(c);
	}

	void setStr(const char* s) {
		segidx[0]->setValue(s[0]);
		segidx[1]->setValue(s[1]);
	}

	void setStr(String s) {
		segidx[0]->setValue(s[0]);
		segidx[1]->setValue(s[1]);
	}

	void debug() {
		segidx[0]->setRaw(0xFF);
		segidx[1]->setRaw(0xFF);
		segidx[2]->setRaw(0xFF);
	}
};