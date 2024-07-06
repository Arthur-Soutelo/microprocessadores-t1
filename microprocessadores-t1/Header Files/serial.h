#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(void);
unsigned char uart_receive(void);
void uart_send(unsigned char data);