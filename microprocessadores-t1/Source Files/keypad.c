#include "keypad.h"

void keypad_init(void) {
	// Set columns as outputs (low)
	KEYPAD_COLS_DIR |= (1 << COL1_PIN) | (1 << COL2_PIN) | (1 << COL3_PIN) | (1 << COL4_PIN);
	KEYPAD_COLS_PORT &= ~((1 << COL1_PIN) | (1 << COL2_PIN) | (1 << COL3_PIN) | (1 << COL4_PIN));

	// Set rows as inputs with pull-up resistors
	KEYPAD_COLS_DIR &= ~((1 << ROW1_PIN) | (1 << ROW2_PIN) | (1 << ROW3_PIN) | (1 << ROW4_PIN));
	KEYPAD_COLS_PORT |= (1 << ROW1_PIN) | (1 << ROW2_PIN) | (1 << ROW3_PIN) | (1 << ROW4_PIN);
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
		KEYPAD_COLS_PORT |= (1 << (COL1_PIN + col)); // Set column high

		for (row = 0; row < 4; row++) {
			if (!(KEYPAD_ROWS_PIN & (1 << (ROW1_PIN + row)))) { // Check if key is pressed
				_delay_ms(10); // Debounce delay
				if (!(KEYPAD_ROWS_PIN & (1 << (ROW1_PIN + row)))) { // Check again after debounce
					KEYPAD_COLS_PORT &= ~(1 << (COL1_PIN + col)); // Reset column
					return keys[row][col]; // Return pressed key
				}
			}
		}

		KEYPAD_COLS_PORT &= ~(1 << (COL1_PIN + col)); // Reset column
	}

	return 0; // Return 0 if no key pressed
}
