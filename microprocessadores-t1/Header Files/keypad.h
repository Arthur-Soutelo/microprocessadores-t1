#include "system_libraries.h"

#ifndef KEYPAD_H
	#define KEYPAD_H

	//#define KEYPAD_COLS_PORT PORTA
	//#define KEYPAD_COLS_DIR  DDRA
	//#define KEYPAD_COLS_PIN  PINA

	#define COL1_PIN PA4
	#define COL2_PIN PA5
	#define COL3_PIN PA6
	#define COL4_PIN PA7

	#define ROW1_PIN PA0
	#define ROW2_PIN PA1
	#define ROW3_PIN PA2
	#define ROW4_PIN PA3

	void keypad_init(void);
	char keypad_getkey(void);
	ProductNumber get_product_number(char key);
	void read_card_number(char *card_number) ;
	
	void read_login(char *login_number);
	void read_pwd(const char *login_line, char *pwd);

#endif // KEYPAD_H
