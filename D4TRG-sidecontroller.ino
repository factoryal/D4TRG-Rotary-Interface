#include "pin_definitions.h"
#include "segment.h"
#include "property_controller.h"

SegmentUpdater fnd;
Properties property(&fnd);

#include "encoder.h"


void setup() {
	cli();
	TCCR1B |= 1 << CS11 | 1 << CS10; // clk/8
	TIMSK1 |= 1 << TOIE1;
	sei();

	Serial.begin(9600);


}

void loop() {
	char buffer[3] = "  ";
	unsigned int val = enc.read() % 100;
	if(val<10) itoa(enc.read() % 100, buffer+1, 10);
	else itoa(enc.read() % 100, buffer, 10);
	
	fnd.setStr(buffer);
	fnd.setLevel(map(enc.read() % 100, 0, 95, 0, 8));
}


ISR(TIMER1_OVF_vect) {

	fnd.update();
}