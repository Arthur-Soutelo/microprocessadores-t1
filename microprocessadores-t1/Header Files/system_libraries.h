#ifndef SYSTEM_LIBRARIES_H_
	#define SYSTEM_LIBRARIES_H_
	
	#define MAX_LOGINS 5
	#define LOGIN_SIZE 6
	#define PASSWORD_SIZE 6
	
	#define CARD_NUMBER_LENGTH 6  // Length of the card number (6 digits + null terminator)
	#define MAX_CARDS 10
	
	#define F_CPU 16000000UL  // Define the clock frequency as 16 MHz
	
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/eeprom.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <util/delay.h>

	// Define a structure to hold the keys
	typedef struct {
		char first_key;
		char second_key;
	} ProductNumber;
	
	typedef struct {
		char card_number[7];  // Card number as a string (6 characters + null terminator)
		float balance;        // Balance as a float
	} Card;
	
	typedef struct {
		char login[LOGIN_SIZE];
		char password[PASSWORD_SIZE];
	} UserCredentials;

	
#endif // SYSTEM_LIBRARIES_H_