#ifndef SYSTEM_LIBRARIES_H_
#define SYSTEM_LIBRARIES_H_
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif // SYSTEM_LIBRARIES_H_


#ifndef TIMER_H_
#define TIMER_H_

	void config_timer1_600Hz();
	void config_timer1_2kHz();
	void timer1_stop(void);
	
	void timer0_delay_us(unsigned int microseconds);


#endif /* TIMER_H_ */