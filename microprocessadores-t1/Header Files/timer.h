#include "system_libraries.h"

#ifndef TIMER_H_
	#define TIMER_H_

	#ifndef F_CPU
		#define F_CPU 16000000UL	// 16 MHz clock frequency
	#endif

	#define BUZZER_PIN	PE4
	
	// ================================= Timer 1 ================================= //
	// ========================== TIMEOUT INSERT COINS ========================== //
	
	extern volatile uint8_t TIMEOUT_FLAG;
	void timer1_stop(void);
	void init_timer1(void);
	
	// ================================= Timer 2  ================================= //
	// ========================= OPERATOR LOGIN TIMEOUT ========================= //
	
	void timer2_init(void);
	char is_timeout_2(void);
	void reset_timer_2(void);
	void stop_timer2(void);
	extern volatile uint16_t elapsed_time_2; // Track elapsed time
	
	// ================================= Timer 3  ================================= //
	// ================================= BUZZER ================================= //
	
	void init_door_buzzer(void);
	void sound_alarm(void);
	void stop_alarm(void);
	
	// ================================= Timer 4  ================================= //
	// ================================ DOOR LED ================================ //
	
	void init_timer4(void);
	void init_led_porta(void);
	void blink_led(void);	


#endif /* TIMER_H_ */