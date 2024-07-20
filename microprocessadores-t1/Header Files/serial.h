#include "system_libraries.h"

#ifndef SERIAL_H_
	#define SERIAL_H_
	
	#define BUFFER_SIZE 25
	#define UART_TIMEOUT 500 // Timeout in milliseconds
	
	void uart_init(unsigned long baudrate);
	unsigned char uart_receive(void);
	void uart_send(unsigned char data);
	void uart_send_string(const char *data);
	//ProductNumber send_product_number(char key); //product_number
	void receive_product_data(char *buffer);
	void confirm_cash_purchase(void);
	void handle_purchase_response(char *response);
	void confirm_card_purchase(const char *card_number);
	
	void send_product_selection(ProductNumber product);
	void receive_answer(char *buffer);

	
	//void uart_init(void);
	//unsigned char uart_receive(void);
	//void uart_send(unsigned char data);

#endif // SERIAL_H_