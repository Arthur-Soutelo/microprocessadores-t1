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

#endif // MAIN_HEADER_H_

//#include <EEPROM.h>


