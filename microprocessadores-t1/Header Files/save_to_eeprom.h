#include "system_libraries.h"

#ifndef SAVE_TO_EEPROM_H
	#define SAVE_TO_EEPROM_H
	
	void eeprom_write(unsigned int address, unsigned char data);
	unsigned char eeprom_read(unsigned int address);

#endif // SAVE_TO_EEPROM_H
