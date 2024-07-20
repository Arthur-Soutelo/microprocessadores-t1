#include "system_libraries.h"

#ifndef SERIAL_H_
	#define SERIAL_H_
	
	#define BUFFER_SIZE 35
	#define PRICE_SIZE 10
	#define NAME_SIZE 20 // Adjust based on the expected max name length
	#define UART_TIMEOUT 500 // Timeout in milliseconds
	
	void uart_init(unsigned long baudrate);
	unsigned char uart_receive(void);
	void uart_send(unsigned char data);
	void uart_send_string(const char *data);
	
	void receive_product_data(char *buffer);
	void confirm_cash_purchase(void);
	void handle_purchase_response(char *response);
	void confirm_card_purchase(const char *card_number);
	
	void send_product_selection(ProductNumber product);
	void receive_answer(char *buffer);
	
	void get_name_from_buffer(char *buffer, char *name);

	
	//void uart_init(void);
	//unsigned char uart_receive(void);
	//void uart_send(unsigned char data);

#endif // SERIAL_H_