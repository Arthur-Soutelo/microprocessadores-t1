#include "system_libraries.h"

#ifndef SAVE_TO_EEPROM_H
	#define SAVE_TO_EEPROM_H
	
	#define MAX_CARDS 10
	
	void eeprom_write(unsigned int address, unsigned char data);
	unsigned char eeprom_read(unsigned int address);
	
	void save_card_data(uint8_t index, const char* card_number, float balance);
	Card read_card_data(uint8_t index) ;
	int find_card_index(const char* card_number);
	int add_new_card(const char* card_number, float balance);
	int remove_card(const char* card_number) ;

	void init_base_cards(void);

#endif // SAVE_TO_EEPROM_H
