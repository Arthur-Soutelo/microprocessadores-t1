#include "system_libraries.h"

#ifndef SERIAL_H_
	#define SERIAL_H_
	
	#define N_SEND 3
	
	#define BUFFER_SIZE 50
	#define PRICE_SIZE 10
	#define NAME_SIZE 20		// Adjust based on the expected max name length
	#define UART_TIMEOUT 10000  // Timeout in milliseconds
	
	void uart_init(unsigned long baudrate);
	unsigned char uart_receive(void);
	void uart_send(unsigned char data);
	void uart_send_string(const char *data);
	
	void receive_product_data(char *buffer);
	void handle_purchase_response(char *response);
	void send_confirm_card_purchase(const char *card_number);
	void send_confirm_cash_purchase(void);
	
	void send_product_selection(ProductNumber product);
	void send_add_new_card(const char *card_number);
	void send_update_card_balance(const char *card_number, const char *balance);
	void send_confirm_restock(ProductNumber product, ProductNumber quantity);
	void send_confirm_cash_withdraw(void);
	
	void get_name_from_buffer(unsigned char *buffer, char *name);
	void get_price_from_buffer(unsigned char *buffer, char *price);
	void receive_data_from_uart(unsigned char *buffer);
	
	int uart_ready(void);


	
	//void uart_init(void);
	//unsigned char uart_receive(void);
	//void uart_send(unsigned char data);

#endif // SERIAL_H_