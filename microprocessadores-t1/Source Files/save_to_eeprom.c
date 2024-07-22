#include "save_to_eeprom.h"

extern Card EEMEM eeprom_cards[MAX_CARDS];

void eeprom_write(unsigned int address, unsigned char data) {
	eeprom_update_byte((uint8_t*)address, data);
}

unsigned char eeprom_read(unsigned int address) {
	return eeprom_read_byte((uint8_t*)address);
}	

void save_card_data(uint8_t index, const char* card_number, float balance) {
	if (index < MAX_CARDS) {
		Card card;
		strncpy(card.card_number, card_number, sizeof(card.card_number) - 1);
		card.card_number[sizeof(card.card_number) - 1] = '\0';  // Ensure null-termination
		card.balance = balance;
		eeprom_update_block((const void*)&card, (void*)&eeprom_cards[index], sizeof(Card));
	}
}


Card read_card_data(uint8_t index) {
	Card card = {0};
	if (index < MAX_CARDS) {
		eeprom_read_block((void*)&card, (const void*)&eeprom_cards[index], sizeof(Card));	
	}
	return card;
}
