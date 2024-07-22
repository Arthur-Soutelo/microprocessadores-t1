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
		char card_number[7];  // Card number as a string (6 characters + null terminator)
		float balance;        // Balance as a float
	} Card;
	
	#define F_CPU 16000000UL  // Define the clock frequency as 16 MHz
	#include <util/delay.h>
	
	#define CARD_NUMBER_LENGTH 6  // Length of the card number (6 digits + null terminator)
	
#endif // SYSTEM_LIBRARIES_H_