#include "serial.h"
#include "keypad.h"

void uart_init(unsigned long baudrate) {
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	unsigned int ubrr = F_CPU/16/baudrate - 1;
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
}

//unsigned char uart_receive_no_timeout(void) {
	//// Wait for data to be received
	//while (!(UCSR0A & (1 << RXC0)));
	//return UDR0;
//}

unsigned char uart_receive(void) {
	unsigned int elapsed_time = 0;
	while (!(UCSR0A	& (1 << RXC0))) { // Wait for data to be received
		_delay_ms(1); // Wait 1 ms
		elapsed_time++;
		if (elapsed_time >= UART_TIMEOUT) {
			//return -1; // Return -1 if timeout occurs
			return 0xFF;
		}
	}	
	return UDR0; // Get and return received data from buffer
}

void uart_send(unsigned char data) {
	unsigned char elapsed_time = 0;

	// Wait for the empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0))) {
		_delay_ms(1); // Wait 1 ms
		elapsed_time++;
		if (elapsed_time >= UART_TIMEOUT) {
			// Timeout occurred, handle it here if needed
			return;
		}
	}
	
	// Put data into buffer, sends the data
	UDR0 = data;
}

void uart_send_string(const char *data) {
	while (*data) {
		uart_send(*data++);
	}
}

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
	//for (int i = 0; i < BUFFER_SIZE; i++) {
		////uart_send(buffer[i]);
		//buffer[i] = 0;
	//}
	
	// Receive the first two characters with timeout
	buffer[0] = uart_receive();					// 'A' - Aplicativo
	buffer[1] = uart_receive();
	if (buffer[0] == 0xFF || buffer[1] == 0xFF) {
		return;	// Handle timeout error
	}


	//// Receive the first two characters
	//buffer[0] = uart_receive(); // 'A' - Aplicativo
	//buffer[1] = uart_receive();

	switch (buffer[0]) {
		case 'A':
		switch (buffer[1]) {
			case 'P': { // Product Information
				buffer[2] = uart_receive(); // Size of data (22 or 16)
				unsigned char data_size = buffer[2];
				if (data_size <= BUFFER_SIZE - 3) { // Prevent buffer overrun
					for (unsigned char i = 0; i < data_size; i++) {
						buffer[3 + i] = uart_receive();
					}
				}
			} break;

			case 'E': { // Purchase Result - Cash
				buffer[2] = uart_receive(); // Response
				// '0' - Compra efetivada com sucesso				// '1' - Compra com falha (produto inválido)
				// '2' - Compra com falha (quantidade insuficiente)
				// '3' - Compra com falha (validade vencida)
			} break;

			case 'C': { // Purchase Result - Card
				buffer[2] = uart_receive(); // Response
				// '0' - Compra efetivada com sucesso				// '1' - Compra com falha (produto inválido)
				// '2' - Compra com falha (quantidade insuficiente)
				// '3' - Compra com falha (validade vencida
				// '4' - Compra com falha (cartão inválido)
			} break;

			case 'Q': { // Quantity - Result
				buffer[2] = uart_receive(); // Response
				// '0' - Compra efetivada com sucesso				// '1' - Compra com falha (produto inválido)
				// '2' - Compra com falha (quantidade inválida)
			} break;
			
			case 'R': { // Cash Removal - Result
			} break;
			
			case 'A': { // Update Card - Result
				buffer[2] = uart_receive(); // Response
				// '0' - Atualização efetivada com sucesso				// '1' - Atualização com falha (valor inválido)
				// '4' - Atualização com falha (cartão inválido)
			} break;

			case 'I': { // Add Card - Result
			} break;

			default:
			// Handle unexpected input
			break;
		}
		break;

		default:
		// Handle unexpected input
		break;
	}
}

	


//void receive_product_data(char *buffer) {
	//buffer[0] = uart_receive(); // 'A'
	//buffer[1] = uart_receive(); // 'P'
	//buffer[2] = uart_receive(); // Size of data (22 or 16)
	//for (int i = 0; i < buffer[2]; i++) {
		//buffer[3 + i] = uart_receive();
	//}
	//buffer[3 + buffer[2]] = '\0';
//}

void confirm_cash_purchase(void) {
	uart_send('V');
	uart_send('E');
}

//void handle_purchase_response(char *response) {
	//response[0] = uart_receive(); // 'A'
	//response[1] = uart_receive(); // 'E'
	//response[2] = uart_receive(); // Result code ('0', '1', '2', '3')
	//response[3] = '\0';
//}

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

void get_name_from_buffer(char *buffer, char *name) {
	int data_size = buffer[2]; // Size of data
	int name_start_index = 3; // Assuming name starts at the 4th byte

	for (int i = 0; i < data_size - 3 && i < NAME_SIZE - 1; i++) {
		name[i] = buffer[name_start_index + i];
		if (buffer[name_start_index + i] == '\0') {
			break; // Stop copying if null terminator is encountered
		}
	}
	name[data_size - 3 < NAME_SIZE - 1 ? data_size - 3 : NAME_SIZE - 1] = '\0'; // Null terminate the string
}


void get_price_from_buffer(char *buffer, char *price) {
	int data_size = buffer[2];
	int name_start_index = 3;
	int name_end_index = name_start_index;
	int i;

	// Find the end of the name (null terminator)
	for (i = 0; i < data_size - 3 && buffer[name_end_index] != '\0'; i++) {
		name_end_index++;
	}

	// Skip the null terminator
	int price_start_index = name_end_index + 1;

	// Copy the price to the price buffer
	for (i = 0; i < data_size - price_start_index && i < PRICE_SIZE - 1; i++) {
		price[i] = buffer[price_start_index + i];
		if (buffer[price_start_index + i] == '\0') {
			break;
		}
	}
	price[i] = '\0';
}

void receive_data_from_uart(char *buffer) {
	// Clear the buffer (optional)
	for (int i = 0; i < BUFFER_SIZE; i++) {
		//uart_send(buffer[i]);
		buffer[i] = 0;
	}
	
	unsigned int i = 0;
	char received_char;
	
	while (i < BUFFER_SIZE - 1) { // Leave space for null terminator
		received_char = uart_receive(); // Function to receive a character
		if (received_char != 0xFF) { // Check for timeout or valid data
			if (received_char == '\n') { // End of line or message
				break;
			}
			buffer[i++] = received_char;
		}
	}
	
	buffer[i] = '\0'; // Null-terminate the string
}