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

int uart_ready(void){
	return ((UCSR0A & (1 << RXC0)) && UDR0 != 0xFF);
}

unsigned char uart_receive(void) {
	while (!(UCSR0A	& (1 << RXC0)));
	return UDR0; // Get and return received data from buffer
}


void uart_send(const unsigned char data) {
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
//
//void uart_send_string_2(const char *data, char length) {
	//for (char i = 0; i < length; i++) {
		//uart_send(data[i]);
	//}
//}

void send_product_selection(ProductNumber product){
	for(char i=0; i<N_SEND; i++){
		// Send Code
		uart_send('V');
		uart_send('P');
		// Use the product number (first_key and second_key)
		uart_send(product.first_key);
		uart_send(product.second_key);
	}
}

void send_confirm_cash_purchase(void){
	//for(char i=0; i<N_SEND; i++){
		// Send Code
		uart_send('V');
		uart_send('E');
	//}
}

void send_confirm_card_purchase(const char *num){
	//for(char i=0; i<N_SEND; i++){
		// Send Code
		uart_send('V');
		uart_send('C');	
		for (int i = 0; i < 6; i++) {
			uart_send(num[i]);
		}
	//}
}

void send_add_new_card(const char *card_number) {
	//for(char i=0; i<N_SEND; i++){
		uart_send('V');
		uart_send('I');
		uart_send(card_number[0]);
		uart_send(card_number[1]);
		uart_send(card_number[2]);
		uart_send(card_number[3]);
		uart_send(card_number[4]);
		uart_send(card_number[5]);
	//}
}

void send_update_card_balance(const char *card_number, const char *balance) {
	//for(char i=0; i<N_SEND; i++){
		uart_send('V');
		uart_send('A');
		uart_send(card_number[0]);
		uart_send(card_number[1]);
		uart_send(card_number[2]);
		uart_send(card_number[3]);
		uart_send(card_number[4]);
		uart_send(card_number[5]);
		uart_send(balance[0]);
		uart_send(balance[1]);
	//}
}

void send_confirm_restock(const char *product, const char *quantity){
	//for(char i=0; i<N_SEND; i++){
		uart_send('V');
		uart_send('Q');
		uart_send(product[0]);
		uart_send(product[1]);
		uart_send(quantity[0]);
		uart_send(quantity[1]);
	//}
}

void send_confirm_cash_withdraw(void) {
	for(char i=0; i<N_SEND; i++){
		uart_send('V');
		uart_send('R');
	}
}

// ---------------------------------------------------------- //

void get_name_from_buffer(unsigned char *buffer, char *name) {
	int data_size = buffer[2]; // Size of data
	int name_start_index = 3; // Assuming name starts at the 4th byte

	for (int i = 0; i < data_size - 3 ; i++) {
		name[i] = buffer[name_start_index + i];
		if (buffer[name_start_index + i] == '\0') {
			break; // Stop copying if null terminator is encountered
		}
	}
}


void get_price_from_buffer(unsigned char *buffer, char *price) {
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
	for (i = 0; i < data_size - (price_start_index-3) && i < PRICE_SIZE - 1; i++) {
		price[i] = buffer[price_start_index + i];
		if (buffer[price_start_index + i] == '\0') {
			break;
		}
	}
	price[i] = '\0';
}

void receive_data_from_uart(unsigned char *buffer) {
	// Clear the buffer (optional)
	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0xFF;
	}
	
	buffer[0] = uart_receive();
	buffer[1] = uart_receive();
	buffer[2] = uart_receive();
	
	if(buffer[0]=='A' && buffer[0]=='P'){
		unsigned char i = 3;
		char received_char;
		while (i < buffer[2]) { // Leave space for null terminator
			received_char = uart_receive(); // Function to receive a character
			if (received_char != 0xFF) {
				buffer[i++] = received_char;
			}
		}
		buffer[i] = '\0'; // Null-terminate the string
	}
	else{
		buffer[3] = uart_receive();
		buffer[4] = '\0'; // Null-terminate the string
	}
}