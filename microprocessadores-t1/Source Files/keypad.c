/*****************************************************************************/
/*					         KEYPAD FUNCTIONS   							 */
/*****************************************************************************/

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
	char index = 0;

	// Initialize card_number buffer
	memset(card_number, 0, 6 + 1);

	while (index < 6) {
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
				memset(card_number, 0, 6 + 1);
				index = 0; // Reset index
				clear_display();
				write_string_line(1,"Digite o Cartao:");
				write_string_line(2,card_number);
			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}

	card_number[index] = '\0'; // Null-terminate the card number
}

float read_card_balance(char *card_balance) {
	char key;
	char index = 0;
	float balance_value;

	// Initialize card_number buffer
	memset(card_balance, 0, 2 + 1);

	while (index < 2) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				card_balance[index++] = key; // Store the digit in the card_number buffer
				//write_data_LCD(key);
				clear_display();
				write_string_line(1,"Digite o Valor:");
				write_string_line(2,card_balance);
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				// Optionally, clear the card_number buffer
				memset(card_balance, 0, 2 + 1);
				index = 0; // Reset index
				clear_display();
				write_string_line(1,"Digite o Valor:");
				write_string_line(2,card_balance);
			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}
	card_balance[index] = '\0'; // Null-terminate the card number
	
	balance_value = atof(card_balance);
	if (balance_value <= 70.0) {
		return balance_value;
		} else {
		clear_display();
		write_string_line(1,"Valor Invalido!");
		_delay_ms(2000); // Exibe a mensagem de erro por 2 segundos
		return 0; // Reseta o valor do saldo para 0
	}
	
}

// Function to read login number from the keypad
void read_login(char *login_number) {
	char key;
	char index_login = 0;
	
	clear_display();
	write_string_line(1,"User: ");
	write_string_line(2,"Pwd : ");

	// Initialize login buffer
	memset(login_number, 0, 6 + 1);
	
	while (index_login < 6) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				login_number[index_login++] = key; // Store the digit in the card_number buffer
				clear_display();
				write_string_line(1,"User: ");
				write_string_LCD(login_number);
				write_string_line(2,"Pwd : ");
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				// Optionally, clear the card_number buffer
				memset(login_number, 0, 6 + 1);
				index_login = 0; // Reset index
				clear_display();
				write_string_line(1,"User: ");
				write_string_LCD(login_number);
				write_string_line(2,"Pwd : ");

			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}
	login_number[index_login] = '\0'; // Null-terminate the card number
	
}
// Function to read a card number from the keypad
void read_pwd(const char *login_line, char *pwd) {
	char key;
	char index_pwd = 0;
	
	clear_display();
	write_string_line(1,login_line);
	write_string_line(2,"Pwd : ");

	// Initialize pwd buffer
	memset(pwd, 0, 6 + 1);
	
	while (index_pwd < 6) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				pwd[index_pwd++] = key; // Store the digit in the card_number buffer
				clear_display();
				write_string_line(1,login_line);
				write_string_line(2,"Pwd : ");
				for (char i = 0; i < index_pwd; i++) {
					write_string_LCD("*"); // Mask the password input
				}
				//write_string_LCD(pwd);
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				// Optionally, clear the card_number buffer
				memset(pwd, 0, 6 + 1);
				index_pwd = 0; // Reset index
				clear_display();
				write_string_line(1,login_line);
				write_string_line(2,"Pwd : ");
				write_string_LCD(pwd);

			}
			_delay_ms(100); // Adjust delay as needed
		}
	}
	pwd[index_pwd] = '\0'; // Null-terminate the card number
}

// Função para coletar o n° do produto (2 dígitos)
void read_product(char *product) {
	char key;
	char index_product = 0;
	
	clear_display();
	write_string_line(1,"Num. produto: ");
	write_string_line(2,"Quantidade: ");
	
	// Initialize product buffer
	memset(product, 0, 2 + 1);
	
	while (index_product < 2) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') { // Check if the key is a digit
				product[index_product++] = key; // Store the digit in the product number buffer
				clear_display();
				write_string_line(1,"Num. produto: ");
				write_string_LCD(product);
				write_string_line(2,"Quantidade: ");
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				index_product = 0; // Reset index
				clear_display();
				write_string_line(1,"Num. produto: ");
				write_string_LCD(product);
				write_string_line(2,"Quantidade: ");
			}
			// Add a small delay to debounce
			_delay_ms(100); // Adjust delay as needed
		}
	}
	product[index_product] = '\0'; // Null-terminate the product number
}

// Função para coletar a quantidade (2 dígitos)
void read_quantity(const char *product_line, char *quantity) {
	char key;
	char index_qtd = 0;
	
	// Initialize quantity buffer
	memset(quantity, 0, 2 + 1);
	
	while (index_qtd < 2) {
		key = keypad_getkey();
		if (key != 0) { // Check if a key is pressed
			if (key >= '0' && key <= '9') {
				quantity[index_qtd++] = key; // Store the digit in the quantity buffer
				clear_display();
				write_string_line(1,product_line);
				write_string_line(2,"Quantidade : ");
				write_string_LCD(quantity);
				} else if (key == '#') { // Use '#' as an enter key
				break; // Exit loop when '#' is pressed
				} else if (key == '*') { // Use '*' to cancel input
				index_qtd = 0; // Reset index
				clear_display();
				write_string_line(1,product_line);
				write_string_line(2,"Quantidade : ");
				write_string_LCD(quantity);
			}
			_delay_ms(100); // Adjust delay as needed
		}
	}
	quantity[index_qtd] = '\0'; // Null-terminate the quantity number
}