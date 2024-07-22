#include "system_libraries.h"

#ifndef TIMER_H_
	#define TIMER_H_

	#ifndef F_CPU
		#define F_CPU 16000000UL	// 16 MHz clock frequency
	#endif

	#define BUZZER_PIN	PE4

	void config_timer1_600Hz();
	void config_timer1_2kHz();
	void timer1_stop(void);
	
	void timer0_delay_us(unsigned int microseconds);

	void init_buzzer(void);
	void sound_alarm(void);
	void stop_alarm(void);
		
	extern volatile uint8_t TIMEOUT_FLAG;
	void init_timer1(void);
	
	


#endif /* TIMER_H_ */