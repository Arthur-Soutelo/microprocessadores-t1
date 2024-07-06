#include <avr/io.h>
#include <avr/interrupt.h>


#ifndef TIMER_H_
#define TIMER_H_


void config_timer_600Hz();
void config_timer_2kHz();

void timer1_stop(void);


#endif /* TIMER_H_ */