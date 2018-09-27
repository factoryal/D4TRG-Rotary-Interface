#pragma once

#include "Arduino.h"

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "Encoder.h"



class Rotary {
private:
	long rotval = 0;
	Encoder e = Encoder(PIN_RA, PIN_RB);

public:
	Rotary() {

	}

	void update() {
		rotval = e.read() / 4;
	}

	long read() {
		return rotval;
	}

	void zero() {
		e.write(0);
	}
} enc;

