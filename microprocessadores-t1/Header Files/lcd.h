#ifndef SYSTEM_LIBRARIES_H_
#define SYSTEM_LIBRARIES_H_
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif // SYSTEM_LIBRARIES_H_


#ifndef LCD_H_
#define LCD_H_

	#define RS_PIN     PB0
	#define E_PIN      PB1
	#define D4_PIN     PB2
	#define D5_PIN     PB3
	#define D6_PIN     PB4
	#define D7_PIN     PB5

	#define LCD_PORT   PORTB
	#define LCD_DDR    DDRB

	void init_LCD(void);
	void write_data_LCD(char data);
	void write_command_LCD(char command);
	void clear_display(void);
	void write_string(char line, char text[]);
	void delay_ms(unsigned int ms);

#endif // LCD_H_