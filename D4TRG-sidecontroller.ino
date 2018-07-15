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
	attachInterrupt(digitalPinToInterrupt(PIN_SW), btn, FALLING);

}

void loop() {
	/*char buffer[3] = "  ";
	unsigned int val = enc.read() % 100;
	if(val<10) itoa(val, buffer+1, 10);
	else itoa(val, buffer, 10);
	
	Serial.println(enc.read());
	fnd.setStr(buffer);
	fnd.setLevel(map(val, 0, 95, 0, 8));*/
}

void btn() {

}


ISR(TIMER1_OVF_vect) {
	static byte a = 0;
	if (a++ & 0x02) fnd.update();
	property.update();
}