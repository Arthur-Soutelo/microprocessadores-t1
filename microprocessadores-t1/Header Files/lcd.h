#include "system_libraries.h"

#ifndef LCD_H_
	#define LCD_H_

	#define RS_PIN     PB0
	#define E_PIN      PB1
	
	#define D4_PIN     PB7
	#define D5_PIN     PB6
	#define D6_PIN     PB5
	#define D7_PIN     PB4

	//#define LCD_PORT   PORTB
	//#define LCD_DDR    DDRB
//
	//void init_LCD(void);
	//void write_data_LCD(unsigned char data);
	//void write_command_LCD(unsigned char command);
	//void clear_display(void);
	//void write_string(char line, char *text);
	//void delay_ms(unsigned int ms);

	void init_LCD(void);
	void write_command_LCD(uint8_t command);
	void write_data_LCD(uint8_t data);
	void write_string_LCD(const char *text);
	
	void clear_display(void);
	void write_string_line(char line, const char *text);

#endif // LCD_H_
