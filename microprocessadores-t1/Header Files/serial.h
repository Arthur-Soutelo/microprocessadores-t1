#include "system_libraries.h"

#ifndef SERIAL_H_
	#define SERIAL_H_
	
	void uart_init(unsigned long baudrate);
	unsigned char uart_receive(void);
	void uart_send(unsigned char data);
	
	//void uart_init(void);
	//unsigned char uart_receive(void);
	//void uart_send(unsigned char data);

#endif // SERIAL_H_