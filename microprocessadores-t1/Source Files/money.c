#include "money.h"

static unsigned char debounce(unsigned char pin);
Card EEMEM eeprom_cards[10]; // Armazena os cartões na EEPROM


// Function to handle button clicks and update the total sum
void update_total_sum(float *total_sum) {
	if (button50c_clicked()) {
		*total_sum += 0.50;  // Add 50 cents
	}
	if (button1r_clicked()) {
		*total_sum += 1.00;  // Add 1 dollar
	}
}

void buttons_init(void) {
	// Set button pins as inputs with pull-down resistors
	DDRH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
	PORTH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
}


void door_init(void) {
	DDRH &= ~((1 << DOOR_PIN));
	PORTH &= ~((1 << DOOR_PIN));
}

unsigned char button50c_clicked(void) {
	return debounce(BUTTON1_PIN);
}

unsigned char button1r_clicked(void) {
	return debounce(BUTTON2_PIN);
}

static unsigned char debounce(unsigned char pin) {
	unsigned char count = 0;
	unsigned char keylast = 0;
	unsigned char keynow = 1;

	while (count < 7) {
		_delay_ms(5);								// Adjust debounce delay as needed
		keynow = (PINH & (1 << pin)) >> pin;	// Read pin state and adjust to 0 or 1

		if (keynow == keylast) {
			count++;
			} else {
			count = 0;
		}

		keylast = keynow;
	}

	return keynow;
}

unsigned char read_door_state(void) {
	// Directly read the state of the switch
	return (PINH & (1 << DOOR_PIN)) >> DOOR_PIN;  // Return 1 if switch is closed (pressed), 0 if open
}

void init_led(void){
	// Configura o pino do LED como saída
	DDRL |= (1 << PL7);
}

void turn_on_led(void){
	// Acende o LED
	PORTL |= (1 << PL7);
}

void turn_off_led(void){
	// Apaga o LED
	PORTL &= ~(1 << PL7);
}

void init_cards() {
	// Cartões pré-cadastrados
	Card default_cards[] = {
		{300123, 30.00},
		{300121, 30.00},
		{250025, 25.00}
	};

	// Escreve os cartões pré-cadastrados na EEPROM
	for (short i = 0; i < 3; i++) {
		eeprom_update_block((const void*)&default_cards[i], (void*)&eeprom_cards[i], sizeof(Card));
	}
}

//short find_card(uint32_t code) {
	//Card temp;
	//for (short i = 0; i < 10; i++) {
		//eeprom_read_block((void*)&temp, (const void*)&eeprom_cards[i], sizeof(Card));
		//if (temp.code == code) {
			//return i;
		//}
	//}
	//return -1; // Cartão não encontrado
//}
//
//float get_card_credit(uint32_t code) {
	//short index = find_card(code);
	//if (index != -1) {
		//Card temp;
		//eeprom_read_block((void*)&temp, (const void*)&eeprom_cards[index], sizeof(Card));
		//return temp.credit;
	//}
	//return -1; // Cartão não encontrado
//}
//
//void update_card_credit(uint32_t code, float new_credit) {
	//short index = find_card(code);
	//if (index != -1) {
		//Card temp;
		//eeprom_read_block((void*)&temp, (const void*)&eeprom_cards[index], sizeof(Card));
		//temp.credit = new_credit;
		//eeprom_update_block((const void*)&temp, (void*)&eeprom_cards[index], sizeof(Card));
	//}
//}
//
//void add_new_card(uint32_t code, float credit) {
	//// Adiciona um novo cartão se houver espaço
	//for (short i = 0; i < 10; i++) {
		//Card temp;
		//eeprom_read_block((void*)&temp, (const void*)&eeprom_cards[i], sizeof(Card));
		//if (temp.code == 0xFFFFFFFF) { // Verifica se o slot está vazio
			//Card new_card = {code, credit};
			//eeprom_update_block((const void*)&new_card, (void*)&eeprom_cards[i], sizeof(Card));
			//break;
		//}
	//}
//}

void display_card_info(short card_index) {
	if (card_index >= 0 && card_index < 10) {
		Card temp;
		eeprom_read_block((void*)&temp, (const void*)&eeprom_cards[card_index], sizeof(Card));
		
		char buffer2[17];
		
		// Exibe o código do cartão
		snprintf(buffer2, sizeof(buffer2), "Card: %06lu", temp.code);
		write_string_line(1, buffer2);

		// Exibe o saldo do cartão
		snprintf(buffer2, sizeof(buffer2), "Balance: %.2f", temp.credit);
		write_string_line(2, buffer2);
	}
}