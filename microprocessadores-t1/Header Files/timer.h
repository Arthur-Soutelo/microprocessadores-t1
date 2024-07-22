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

	void init_timer3_buzzer(void);
	void sound_alarm(void);
	void stop_alarm(void);
		
	extern volatile uint8_t timeout_flag;
	void init_timer1(void);
	void init_timer4(void);
	void init_led_porta(void);
	void blink_led(void);	
	extern volatile uint8_t door_open;
	


#endif /* TIMER_H_ */