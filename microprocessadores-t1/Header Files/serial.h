#include "system_libraries.h"

#ifndef SERIAL_H_
	#define SERIAL_H_
	
	#define BUFFER_SIZE 30
	#define PRICE_SIZE 10
	#define NAME_SIZE 20		
	
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
	void send_confirm_restock(const char *product, const char *quantity);
	void send_confirm_cash_withdraw(void);
	
	void get_name_from_buffer(unsigned char *buffer, char *name);
	void get_price_from_buffer(unsigned char *buffer, char *price);
	void receive_data_from_uart(unsigned char *buffer);

#endif // SERIAL_H_