#ifndef SYSTEM_LIBRARIES_H_
	#define SYSTEM_LIBRARIES_H_
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif // SYSTEM_LIBRARIES_H_


#ifndef KEYPAD_H
	#define KEYPAD_H

	#define KEYPAD_COLS_PORT PORTA
	#define KEYPAD_COLS_DIR  DDRA
	#define KEYPAD_ROWS_PIN  PINA

	#define COL1_PIN PA0
	#define COL2_PIN PA1
	#define COL3_PIN PA2
	#define COL4_PIN PA3

	#define ROW1_PIN PA4
	#define ROW2_PIN PA5
	#define ROW3_PIN PA6
	#define ROW4_PIN PA7

	void keypad_init(void);
	char keypad_getkey(void);

#endif // KEYPAD_H
