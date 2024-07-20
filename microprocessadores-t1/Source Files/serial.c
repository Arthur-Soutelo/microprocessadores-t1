#include "serial.h"


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

unsigned char uart_receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uart_send(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
}

// --------------------------------------------------------------


void send_product_number(const char *product_number) {
	uart_send('V');
	uart_send('P');
	uart_send(product_number[0]);
	uart_send(product_number[1]);
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