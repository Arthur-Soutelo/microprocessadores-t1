#ifndef SAVE_TO_EEPROM_H
	#define SAVE_TO_EEPROM_H

	#include <avr/io.h>

	void eeprom_write(unsigned int address, unsigned char data);
	unsigned char eeprom_read(unsigned int address);

#endif // SAVE_TO_EEPROM_H
