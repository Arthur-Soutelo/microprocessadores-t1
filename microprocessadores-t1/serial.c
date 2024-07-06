#include "serial.h"

void uart_init() {
	// Configurar baud rate 4800
	UBRR0H = 0;
	UBRR0L = 207;
	// Habilitar recepção e transmissão
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	// Configurar formato do frame: 8 bits de dados, 1 bit de parada
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

unsigned char uart_receive(void) {
	// Esperar por dado ser recebido
	while ((UCSR0A & (1<<RXC0)) == 0);
	return UDR0;
}

void uart_send(unsigned char data) {
	// Espera ate que o buffer de transmissão esteja vazio
	while (!(UCSR0A & (1<<UDRE0)));
	// Coloca o dado no buffer, enviando o caractere
	UDR0 = data;
}