#ifndef SYSTEM_LIBRARIES_H_
#define SYSTEM_LIBRARIES_H_
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif // SYSTEM_LIBRARIES_H_


#ifndef SERIAL_H_
#define SERIAL_H_

	void uart_init(void);
	unsigned char uart_receive(void);
	void uart_send(unsigned char data);

#endif // SERIAL_H_