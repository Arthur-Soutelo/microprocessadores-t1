#include "save_to_eeprom.h"

Card EEMEM eeprom_cards[MAX_CARDS];
UserCredentials EEMEM eeprom_users[MAX_LOGINS];

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

int find_card_index(const char* card_number) {
	for (uint8_t i = 0; i < MAX_CARDS; i++) {
		Card card = read_card_data(i);
		if (strcmp(card.card_number, card_number) == 0) {
			return i; // Return the index if card number matches
		}
	}
	return -1; // Return -1 if card number is not found
}

void init_base_cards(void){
	 // Save card data to EEPROM
	 save_card_data(0, "300123", 30.00);
	 save_card_data(1, "300121", 30.00);
	 save_card_data(2, "250025", 25.00);
}


int find_empty_slot(void) {
	for (uint8_t i = 0; i < MAX_CARDS; i++) {
		Card card = read_card_data(i);
		if (card.card_number[0] == '\0') { // Check if the card number is empty (indicating an unused slot)
			return i;
		}
	}
	return -1; // Return -1 if no empty slot is found
}

int add_new_card(const char* card_number, float balance) {
	// Check if the card already exists
	int existing_index = find_card_index(card_number);
	if (existing_index != -1) {
		return -2; // Return -2 if card already exists
	}

	// Find an empty slot in EEPROM
	int empty_slot = find_empty_slot();
	if (empty_slot == -1) {
		return -1; // Return -1 if no empty slot is available
	}

	// Save the new card data
	save_card_data(empty_slot, card_number, balance);
	return empty_slot; // Return the index where the card was added
}

int remove_card(const char* card_number) {
	int index = find_card_index(card_number);
	if (index == -1) {
		return -1; // Return -1 if card number is not found
	}

	// Clear the card data at the found index
	Card empty_card = {{0}, 0.0}; // Create an empty card data structure
	eeprom_update_block((const void*)&empty_card, (void*)&eeprom_cards[index], sizeof(Card));
	return index; // Return the index where the card was removed
}

// Function to update the balance of a card
void update_card_balance(const char *card_number, float new_balance) {
	int index = find_card_index(card_number);
	if (index != -1) {
		// Card found, update balance
		Card card;
		eeprom_read_block(&card, &eeprom_cards[index], sizeof(Card));
		card.balance = new_balance;
		eeprom_update_block(&card, &eeprom_cards[index], sizeof(Card));
		} else {
		// Handle card not found
		// Optionally, add new card if needed
	}
}

// Function to subtract a value from a card balance
void subtract_from_card_balance(const char *card_number, float amount_to_subtract) {
	int index = find_card_index(card_number);
	if (index != -1) {
		// Card found, update balance
		Card card;
		eeprom_read_block(&card, &eeprom_cards[index], sizeof(Card));
		
		if (card.balance >= amount_to_subtract) {
			card.balance -= amount_to_subtract;
			eeprom_update_block(&card, &eeprom_cards[index], sizeof(Card));
			} else {
			// Handle insufficient balance, e.g., print error or notify user
		}
		} else {
		// Handle card not found, e.g., print error or notify user
	}
}

// ------------------------------------------

void save_user_credentials(uint8_t index, const char* login, const char* password) {
	if (index < MAX_LOGINS) {
		UserCredentials user;
		strncpy(user.login, login, LOGIN_SIZE - 1);
		user.login[LOGIN_SIZE - 1] = '\0';  // Ensure null-termination
		strncpy(user.password, password, PASSWORD_SIZE - 1);
		user.password[PASSWORD_SIZE - 1] = '\0';  // Ensure null-termination
		eeprom_update_block((const void*)&user, (void*)&eeprom_users[index], sizeof(UserCredentials));
	}
}

UserCredentials read_user_credentials(uint8_t index) {
	UserCredentials user = {0};
	if (index < MAX_LOGINS) {
		eeprom_read_block((void*)&user, (const void*)&eeprom_users[index], sizeof(UserCredentials));
	}
	return user;
}

char add_new_user(const char* login, const char* password) {
	for (char i = 0; i < MAX_LOGINS; i++) {
		UserCredentials user = read_user_credentials(i);
		if (user.login[0] == '\0') { // Empty slot
			save_user_credentials(i, login, password);
			return i; // Return the index where the user was added
		}
	}
	return -1; // No empty slot found
}


char validate_user(const char* login, const char* password) {
	for (char i = 0; i < MAX_LOGINS; i++) {
		UserCredentials user = read_user_credentials(i);
		if (strcmp(user.login, login) == 0 && strcmp(user.password, password) == 0) {
			return 1; 
		}
	}
	return 0; // Credentials not found
}


void init_operator(void){
	char login[7];
	char pwd[7];
	
	// Copy strings into arrays
	strncpy(login, "012987", 7 - 1);
	login[7 - 1] = '\0';  // Ensure null termination

	strncpy(pwd, "974896", 7 - 1);
	pwd[7 - 1] = '\0';  // Ensure null termination
	
	add_new_user(login, pwd);
}

void erase_eeprom(void) {
	for (uint16_t i = 0; i < 4096; i++) {
		eeprom_write_byte((uint8_t*)i, 0xFF);
	}
}