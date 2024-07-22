#include "system_libraries.h"

#ifndef SERIAL_H_
	#define SERIAL_H_
	
	#define BUFFER_SIZE 25
	#define PRICE_SIZE 10
	#define NAME_SIZE 20 // Adjust based on the expected max name length
	#define UART_TIMEOUT 10000 // Timeout in milliseconds
	
	void uart_init(unsigned long baudrate);
	unsigned char uart_receive(void);
	void uart_send(unsigned char data);
	void uart_send_string(const char *data);
	
	void receive_product_data(char *buffer);
	void confirm_cash_purchase(void);
	void handle_purchase_response(char *response);
	void confirm_card_purchase(const char *card_number);
	
	void send_product_selection(ProductNumber product);
	void send_choice_cash(void);
	void send_choice_card(char *num);
	
	void get_name_from_buffer(char *buffer, char *name);
	void get_price_from_buffer(char *buffer, char *price);
	void receive_data_from_uart(char *buffer);

	
	//void uart_init(void);
	//unsigned char uart_receive(void);
	//void uart_send(unsigned char data);

#endif // SERIAL_H_