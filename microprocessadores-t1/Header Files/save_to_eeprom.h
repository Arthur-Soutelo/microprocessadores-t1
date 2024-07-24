#include "system_libraries.h"

#ifndef SAVE_TO_EEPROM_H
	#define SAVE_TO_EEPROM_H
	
	extern Card eeprom_cards[MAX_CARDS];
	extern UserCredentials eeprom_users[MAX_LOGINS];
	
	void eeprom_write(unsigned int address, unsigned char data);
	unsigned char eeprom_read(unsigned int address);
	
	void save_card_data(uint8_t index, const char* card_number, float balance);
	Card read_card_data(uint8_t index) ;
	int find_card_index(const char* card_number);
	int add_new_card(const char* card_number, float balance);
	int remove_card(const char* card_number) ;
	void update_card_balance(const char *card_number, float new_balance) ;
	void subtract_from_card_balance(const char *card_number, float amount_to_subtract) ;

	void init_base_cards(void);;
	
	void save_user_credentials(uint8_t index, const char* login, const char* password);
	UserCredentials read_user_credentials(uint8_t index);
	char add_new_user(const char* login, const char* password);
	char validate_user(const char* login, const char* password);
	void init_operator(void);
	void clear_eeprom_vectors(void);

#endif // SAVE_TO_EEPROM_H
