#include "system_libraries.h"

#ifndef MAIN_HEADER_H_
	#define MAIN_HEADER_H_

	#define set_bit(reg,bit)	reg |= (1<<bit)			// Macro defined for Setting a bit of any register.
	#define clear_bit(reg,bit)	reg &= ~(1<<bit)		// Macro defined for Clearing a bit of any register.

	#include "serial.h"
	#include "timer.h"
	#include "lcd.h"
	#include "keypad.h"
	#include "money.h"
	#include "save_to_eeprom.h"
	
	void init_components(void);
	int get_coins_menu(float *total_sum, const char *product_price);
	void get_selected_product_menu(char key);
	void get_card_number(char *card_number);
	void get_card_balance(char *card_number, char *card_balance);
	int card_payment_menu(char *card_number, char *product_price);
	int withdraw_menu(char key);
	void analyze_serial_command(unsigned char *buffer, char *product_name, char *product_price, float total_sum, char *card_number);

#endif // MAIN_HEADER_H_

//#include <EEPROM.h>


