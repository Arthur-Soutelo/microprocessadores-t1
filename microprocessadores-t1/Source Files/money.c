#include "money.h"

static unsigned char debounce(unsigned char pin);

void buttons_init(void) {
	// Set button pins as inputs with pull-down resistors
	DDRH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
	PORTH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
}


void door_init(void) {
	DDRH &= ~((1 << DOOR_PIN));
	PORTH &= ~((1 << DOOR_PIN));
}

unsigned char button1_clicked(void) {
	return debounce(BUTTON1_PIN);
}

unsigned char button2_clicked(void) {
	return debounce(BUTTON2_PIN);
}

static unsigned char debounce(unsigned char pin) {
	unsigned char count = 0;
	unsigned char keylast = 0;
	unsigned char keynow = 1;

	while (count < 7) {
		_delay_ms(10);								// Adjust debounce delay as needed
		keynow = (PINH & (1 << pin)) >> pin;	// Read pin state and adjust to 0 or 1

		if (keynow == keylast) {
			count++;
			} else {
			count = 0;
		}

		keylast = keynow;
	}

	return keynow;
}

unsigned char read_door_state(void) {
	// Directly read the state of the switch
	return (PINH & (1 << DOOR_PIN)) >> DOOR_PIN;  // Return 1 if switch is closed (pressed), 0 if open
}