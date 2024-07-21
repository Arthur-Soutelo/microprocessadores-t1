#ifndef SYSTEM_LIBRARIES_H_
	#define SYSTEM_LIBRARIES_H_
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/eeprom.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	
	// Define a structure to hold the keys
	typedef struct {
		char first_key;
		char second_key;
	} ProductNumber;
	
	typedef struct {
		uint32_t code;    // C�digo do cart�o (6 d�gitos, armazenado como inteiro de 32 bits)
		float credit;     // Saldo do cart�o
	} Card;
	
	#define F_CPU 16000000UL  // Define the clock frequency as 16 MHz
	#include <util/delay.h>
	
#endif // SYSTEM_LIBRARIES_H_