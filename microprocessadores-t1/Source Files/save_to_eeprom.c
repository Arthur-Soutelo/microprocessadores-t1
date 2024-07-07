#include "save_to_eeprom.h"
#include <avr/eeprom.h>

void eeprom_write(unsigned int address, unsigned char data) {
	eeprom_update_byte((uint8_t*)address, data);
}

unsigned char eeprom_read(unsigned int address) {
	return eeprom_read_byte((uint8_t*)address);
}
