#include "src/Encoder/Encoder.h"

#define PIN_CLK		8
#define PIN_DATA	7
#define PIN_LATCH	9
#define PIN_S1		10
#define PIN_S2		11
#define PIN_S3		12
#define PIN_RA		2
#define PIN_RB		3
#define PIN_SW		4

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
		0x5F, /* a */
		0x7C, /* b */
		0x58, /* c */
		0x5E, /* d */
		0x7B, /* e */
		0x71, /* f */
		0x6F, /* g */
		0x74, /* h */
		0x10, /* i */
		0x0C, /* j */
		0x75, /* k */
		0x30, /* l */
		0x14, /* m */
		0x54, /* n */
		0x5C, /* o */
		0x73, /* p */
		0x67, /* q */
		0x50, /* r */
		0x6D, /* s */
		0x78, /* t */
		0x1C, /* u */
		0x1C, /* v */
		0x14, /* w */
		0x76, /* x */
		0x6E, /* y */
		0x5B /* z */
	};

public:
	void setValue(byte ascii) {
		// lower to upper
		if (ascii >= 'a' && ascii <= 'z') ascii ^= 0x20;
		
		// mapping
		if (ascii >= '0' && ascii <= '9') ascii -= ('0' - 1);
		else if (ascii >= 'A' && ascii <= 'Z') ascii -= ('A' + 10 - 1);
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
		//if (millis() - oldTime > updatePeriod) {
			digitalWrite(PIN_LATCH, 0);
			for (int i = 0; i < 3; i++) {
				digitalWrite(pinidx[i], 0);
			}
			shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, segidx[currentSegmentIdx]->getValue());
			digitalWrite(PIN_LATCH, 1);

			digitalWrite(pinidx[currentSegmentIdx], 1);
			if (++currentSegmentIdx >= 3) currentSegmentIdx = 0;

			oldTime = millis();
		//}
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
} fnd;

Encoder enc(PIN_RA, PIN_RB);

void setup() {
	cli();
	TCCR1B |= 1 << CS11 | 1 << CS10; // clk/8
	TIMSK1 |= 1 << TOIE1;
	sei();

	Serial.begin(9600);


}

void loop() {
	for (int i = 0; i <= 20; i++) {
		char buffer[3];
		int val = i * 5;
		if (val > 99) val = 99;
		if (val < 10) {
			buffer[0] = '0';
			itoa(val, buffer + 1, 10);
		}
		else itoa(val, buffer, 10);
		fnd.setStr(buffer);
		fnd.setLevel(map(val, 1, 95, 1, 8));
		Serial.println(enc.read());
		delay(300);
	}
}


ISR(TIMER1_OVF_vect) {

	fnd.update();
}