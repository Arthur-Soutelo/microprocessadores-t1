#include "keypad.h"
#include "lcd.h"

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

// Function to get the product number based on the first key
ProductNumber get_product_number(char key) {
	ProductNumber result;
	
	// Wait for the second digit with a timeout
	unsigned int timeout = 2500; // Timeout in milliseconds
	unsigned int elapsed_time = 0;
	
	if (key != '1' && key != '0') {
		result.first_key = '0';
		result.second_key = key;
	}
	else {
		while (result.second_key == 0 && elapsed_time < timeout) {
			result.second_key = keypad_getkey(); // Get the second digit
			_delay_ms(50); // Small delay to debounce and prevent busy-waiting
			elapsed_time += 50;
		}

		if (elapsed_time >= timeout && result.second_key == 0) {
			result.second_key = key;
			result.first_key = '0';
		}
		else if (elapsed_time <= timeout && result.second_key != 0)  {
			result.first_key = key;
		}
	}

	return result;
}


// Function to read a card number from the keypad
void read_card_number(char *card_number) {
	char key;
	uint8_t index = 0;

	// Initialize card_number buffer
	memset(card_number, 0, CARD_NUMBER_LENGTH + 1);

	while (index < CARD_NUMBER_LENGTH) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				card_number[index++] = key; // Store the digit in the card_number buffer
				//write_data_LCD(key);
				clear_display();
				write_string_line(1,"Digite o Cartao:");
				write_string_line(2,card_number);
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				// Optionally, clear the card_number buffer
				memset(card_number, 0, CARD_NUMBER_LENGTH + 1);
				index = 0; // Reset index

			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}

	card_number[index] = '\0'; // Null-terminate the card number
}

// Function to read a card number from the keypad
void read_login(char *login_number, char *pwd) {
	char key;
	uint8_t index = 0;
	
	clear_display();
	write_string_line(1,"User: ");
	write_string_line(2,"Pwd: ");

	// Initialize login buffer
	memset(login_number, 0, CARD_NUMBER_LENGTH + 1);
	while (index < CARD_NUMBER_LENGTH) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				login_number[index++] = key; // Store the digit in the card_number buffer
				//write_data_LCD(key);
				clear_display();
				write_string_line(1,"User: ");
				write_string_LCD(login_number);
				write_string_line(2,"Pwd: ");
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				// Optionally, clear the card_number buffer
				memset(login_number, 0, CARD_NUMBER_LENGTH + 1);
				index = 0; // Reset index

			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}
	login_number[index] = '\0'; // Null-terminate the card number
	
	index = 0;
	
	// Initialize login buffer
	memset(pwd, 0, CARD_NUMBER_LENGTH + 1);
	while (index < CARD_NUMBER_LENGTH) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				pwd[index++] = key; // Store the digit in the card_number buffer
				//write_data_LCD(key);
				clear_display();
				write_string_line(1,"User:");
				write_string_LCD(login_number);
				write_string_line(2,"Pwd: ");
				write_string_LCD(pwd);
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				// Optionally, clear the card_number buffer
				memset(pwd, 0, CARD_NUMBER_LENGTH + 1);
				index = 0; // Reset index

			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}
	pwd[index] = '\0'; // Null-terminate the card number
}