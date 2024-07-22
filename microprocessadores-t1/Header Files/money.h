#include "system_libraries.h"

#ifndef MONEY_H
	#define MONEY_H

	#define MONEY_PORT PORTH
	#define MONEY_DIR  DDRH
	#define MONEY_PIN  PINH

	#define BUTTON1_PIN     PH5 //50 centavos
	#define BUTTON2_PIN     PH6 //1 real

	#define DOOR_PIN	PH4

	void buttons_init(void);
	unsigned char button50c_clicked(void);
	unsigned char button1r_clicked(void);
	
	void door_init(void) ;
	unsigned char read_door_state(void) ;
	
	void update_total_sum(float *total_sum);
	
	//void init_cards();
	//short find_card(uint32_t code);
	//float get_card_credit(uint32_t code);
	//void update_card_credit(uint32_t code, float new_credit);
	//void add_new_card(uint32_t code, float credit);
	//void display_card_info(short card_index);

#endif // MONEY_H
