#ifndef SYSTEM_LIBRARIES_H_
	#define SYSTEM_LIBRARIES_H_	
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif // SYSTEM_LIBRARIES_H_


#ifndef MONEY_H
	#define MONEY_H

	#define MONEY_PORT PORTA
	#define MONEY_DIR  DDRA
	#define MONEY_PIN  PINA

	#define BUTTON1_PIN     PA0 // Assuming Button 1 is connected to PA0
	#define BUTTON2_PIN     PA1 // Assuming Button 2 is connected to PA1

	void buttons_init(void);
	unsigned char button1_clicked(void);
	unsigned char button2_clicked(void);

#endif // MONEY_H
