#include "keypad.h"

// Debounce function to check the key press stability
static unsigned char debounce(unsigned char row, unsigned char col) {
	unsigned char count = 0;
	unsigned char keylast = 0;
	unsigned char keynow = 1;

	while (count < 7) {
		_delay_ms(10); // Adjust debounce delay as needed
		keynow = PINA & (1 << row);

		if (keynow == keylast) {
			count++;
			} else {
			count = 0;
		}

		keylast = keynow;
	}

	return keynow;
}

void keypad_init(void) {
	// Set columns as outputs (low)
	DDRA |= (1 << COL1_PIN) | (1 << COL2_PIN) | (1 << COL3_PIN) | (1 << COL4_PIN);
	PORTA &= ~((1 << COL1_PIN) | (1 << COL2_PIN) | (1 << COL3_PIN) | (1 << COL4_PIN));

	// Set rows as inputs with pull-up resistors
	DDRA &= ~((1 << ROW1_PIN) | (1 << ROW2_PIN) | (1 << ROW3_PIN) | (1 << ROW4_PIN));
	PORTA |= (1 << ROW1_PIN) | (1 << ROW2_PIN) | (1 << ROW3_PIN) | (1 << ROW4_PIN);
}

char keypad_getkey(void) {
	unsigned char row, col;
	const char keys[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};

	for (col = 0; col < 4; col++) {
		// Set the current column to low
		PORTA &= ~(1 << (COL1_PIN + col));

		for (row = 0; row < 4; row++) {
			if (!(PINA & (1 << (ROW1_PIN + row)))) { // Check if the key is pressed
				if (debounce(ROW1_PIN + row, COL1_PIN + col)) { // Check if key is pressed and debounced
					// Reset the column to high
					PORTA |= (1 << (COL1_PIN + col));
					return keys[row][col]; // Return the pressed key
				}
			}
		}

		// Reset the column to high
		PORTA |= (1 << (COL1_PIN + col));
	}

	return 0; // Return 0 if no key is pressed
}
