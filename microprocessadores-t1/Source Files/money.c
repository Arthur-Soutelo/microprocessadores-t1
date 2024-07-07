#include "money.h"

static unsigned char debounce(unsigned char pin);

void buttons_init(void) {
	// Set button pins as inputs with pull-down resistors
	MONEY_DIR &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
	MONEY_PORT &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
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
		_delay_ms(2);								// Adjust debounce delay as needed
		keynow = (MONEY_PIN & (1 << pin)) >> pin;	// Read pin state and adjust to 0 or 1

		if (keynow == keylast) {
			count++;
			} else {
			count = 0;
		}

		keylast = keynow;
	}

	return keynow;
}
