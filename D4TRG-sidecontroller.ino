#include "pin_definitions.h"
#include "segment.h"


SegmentUpdater fnd;


#include "rotary.h"
#include "property_controller.h"
Properties property(&fnd, &enc);

void setup() {
	cli();
	TCCR1B; // clk/8
	TIMSK1 |= 1 << TOIE1;
	sei();

	Serial.begin(9600);
}

void loop() {
}

void btn() {
	
}


ISR(TIMER1_OVF_vect) {
	static byte a = 0;
	if (a++ & 0x02) fnd.update();
	property.update();
}