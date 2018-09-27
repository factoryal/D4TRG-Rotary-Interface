
#include "pin_definitions.h"

#include <SoftwareSerial.h>
SoftwareSerial Serial1(PIN_RX1, PIN_TX1);

#include "segment.h"
SegmentUpdater fnd;

#include "rotary.h"
#include "volume_controller.h"
#include "property_controller.h"
Properties property(&fnd, &enc, &Serial1);

void setup() {
	cli();
	TCCR1B; // clk/8
	TIMSK1 |= 1 << TOIE1;
	sei();

	Serial.begin(9600);
}

void loop() {
}

ISR(TIMER1_OVF_vect) {
	static byte a = 0;
	if (a++ & 0x02) fnd.update();
	property.update();
}