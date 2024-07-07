#include "system_libraries.h"

#ifndef MONEY_H
	#define MONEY_H

	#define MONEY_PORT PORTH
	#define MONEY_DIR  DDRH
	#define MONEY_PIN  PINH

	#define BUTTON1_PIN     PH5 // Assuming Button 1 is connected to PA0
	#define BUTTON2_PIN     PH6 // Assuming Button 2 is connected to PA1

	void buttons_init(void);
	unsigned char button1_clicked(void);
	unsigned char button2_clicked(void);

#endif // MONEY_H
