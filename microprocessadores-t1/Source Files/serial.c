#include "serial.h"
#include "keypad.h"

void uart_init(unsigned long baudrate) {
	unsigned int ubrr = F_CPU/16/baudrate - 1;
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

//unsigned char uart_receive_no_timeout(void) {
	//// Wait for data to be received
	//while (!(UCSR0A & (1 << RXC0)));
	//return UDR0;
//}

unsigned char uart_receive(void) {
	unsigned int elapsed_time = 0;
	while (!(UCSR0A & (1 << RXC0))) { // Wait for data to be received
		_delay_ms(1); // Wait 1 ms
		elapsed_time++;
		if (elapsed_time >= UART_TIMEOUT) {
			return -1; // Return -1 if timeout occurs
		}
	}
	return UDR0; // Get and return received data from buffer
}

void uart_send(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
}

void uart_send_string(const char *data) {
	while (*data) {
		uart_send(*data++);
	}
}

// --------------------------------------------------------------

//// Function to get the product number based on the first key
//ProductNumber send_product_number(char key) {
	//ProductNumber result;
	//
	//// Wait for the second digit with a timeout
	//unsigned int timeout = 2500; // Timeout in milliseconds
	//unsigned int elapsed_time = 0;
	//
	//if (key != '1' && key != '0') {
		//result.first_key = '0'; 
		//result.second_key = key;
	//}
	//else {
		//while (result.second_key == 0 && elapsed_time < timeout) {
			//result.second_key = keypad_getkey(); // Get the second digit
			//_delay_ms(50); // Small delay to debounce and prevent busy-waiting
			//elapsed_time += 50;
		//}
//
		//if (elapsed_time >= timeout && result.second_key == 0) {
			//result.second_key = key;
			//result.first_key = '0';
		//}
		//else if (elapsed_time <= timeout && result.second_key != 0)  {
			//result.first_key = key;
		//}
	//}
//
	//return result;
//}

void send_product_selection(ProductNumber product){
	// Send Code
	uart_send('V');
	uart_send('P');
	// Use the product number (first_key and second_key)
	uart_send(product.first_key);
	uart_send(product.second_key);
	
}

void send_choice_cash(void){
	// Send Code
	uart_send('V');
	uart_send('P');	
}
void send_choice_card(char *num){
	// Send Code
	uart_send('V');
	uart_send('c');	
	for (int i = 0; i < 6; i++) {
		uart_send(num[i]);
	}
}

void receive_answer(char *buffer) {
	// Clear the buffer (optional)
	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	
	// Receive the first two characters with timeout
	buffer[0] = uart_receive(); // 'A' - Aplicativo
	if (buffer[0] == -1) {
		// Handle timeout error
		return;
	}
	buffer[1] = uart_receive();
	if (buffer[1] == -1) {
		// Handle timeout error
		return;
	}
	//// Receive the first two characters
	//buffer[0] = uart_receive(); // 'A' - Aplicativo
	//buffer[1] = uart_receive();

	switch (buffer[0]) {
		case 'A':
		switch (buffer[1]) {
			case 'P': { // Product Information
				buffer[2] = uart_receive(); // Size of data (22 or 16)
				int data_size = buffer[2];
				
				if (data_size <= BUFFER_SIZE - 3) { // Prevent buffer overrun
					for (int i = 0; i < data_size; i++) {
						buffer[3 + i] = uart_receive();
					}
				}
			}
			break;

			case 'E': // Purchase Result - Cash
			// Add your handling code here
			break;

			case 'C': // Purchase Result - Card
			// Add your handling code here
			break;

			case 'Q': // Quantity - Result
			// Add your handling code here
			break;

			case 'A': // Update Card - Result
			// Add your handling code here
			break;

			case 'I': // Add Card - Result
			// Add your handling code here
			break;

			default:
			// Handle unexpected input
			break;
		}
		break;

		default:
		
		break;
	}
}

	


void receive_product_data(char *buffer) {
	buffer[0] = uart_receive(); // 'A'
	buffer[1] = uart_receive(); // 'P'
	buffer[2] = uart_receive(); // Size of data (22 or 16)
	for (int i = 0; i < buffer[2]; i++) {
		buffer[3 + i] = uart_receive();
	}
	buffer[3 + buffer[2]] = '\0';
}

void confirm_cash_purchase(void) {
	uart_send('V');
	uart_send('E');
}

void handle_purchase_response(char *response) {
	response[0] = uart_receive(); // 'A'
	response[1] = uart_receive(); // 'E'
	response[2] = uart_receive(); // Result code ('0', '1', '2', '3')
	response[3] = '\0';
}

void confirm_card_purchase(const char *card_number) {
	uart_send('V');
	uart_send('C');
	uart_send(card_number[0]);
	uart_send(card_number[1]);
	uart_send(card_number[2]);
	uart_send(card_number[3]);
	uart_send(card_number[4]);
	uart_send(card_number[5]);
}


//void uart_init(void) {
	//// Configurar baud rate 4800
	//UBRR0H = 0;
	//UBRR0L = 207;
	//// Habilitar recepção e transmissão
	//UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//// Configurar formato do frame: 8 bits de dados, 1 bit de parada
	//UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
//}
//
//unsigned char uart_receive(void) {
	//// Esperar por dado ser recebido
	//while ((UCSR0A & (1<<RXC0)) == 0);
	//return UDR0;
//}
//
//void uart_send(unsigned char data) {
	//// Espera ate que o buffer de transmissão esteja vazio
	//while (!(UCSR0A & (1<<UDRE0)));
	//// Coloca o dado no buffer, enviando o caractere
	//UDR0 = data;
//}