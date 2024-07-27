#include "system_libraries.h"

#ifndef MONEY_H
	#define MONEY_H

	#define MONEY_PORT PORTH
	#define MONEY_DIR  DDRH
	#define MONEY_PIN  PINH

	#define BUTTON1_PIN     PH5 //50 centavos
	#define BUTTON2_PIN     PH6 //1 real

	void buttons_init(void);
	unsigned char button50c_clicked(void);
	unsigned char button1r_clicked(void);
	
	void door_init(void) ;
	char read_door_state(void) ;
	
	void update_total_sum(float *total_sum);
	void init_led(void);
	void turn_on_led(void);
	void turn_off_led(void);

#endif // MONEY_H
