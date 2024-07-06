#include "timer.h"

unsigned short MY_VARIABLE_TCNT1 = 52202;

// ####### Timer 1 => 600Hz ####### //
// Para 600 Hz: TCNT1 = 65535 - (F_CPU / (600 * 2)) = 65535 - 13333 = 52202
void config_timer1_600Hz(){
	TCCR1A = 0;		// Modo Normal
	TCCR1B = 1;	// Sem Prescaler
	MY_VARIABLE_TCNT1 = 52202;
	TCNT1 = MY_VARIABLE_TCNT1;		// = 65.535 - 13 333
	TIMSK1 = (1<<0); // Ativa interrupção por overflow TIMER 1

}

// ####### Timer 1 => 2000Hz ####### //
// Para 2000 Hz: TCNT1 = 65535 - (F_CPU / (2000 * 2)) = 65535 - 4000 = 61535
void config_timer1_2kHz(){
	TCCR1A = 0;		// Modo Normal
	TCCR1B = 1;	// Sem Prescaler
	MY_VARIABLE_TCNT1 = 61535;
	TCNT1 = MY_VARIABLE_TCNT1;		// = 65.535 - 4000
	TIMSK1 = (1<<0); // Ativa interrupção por overflow TIMER 1

}

void timer1_stop(void) {
	// Parar Timer1
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
	// Garantir que o pino PB5 esta em nivel baixo
	PORTB &= ~(1<<PB5);
}


void timer0_delay_us(unsigned int microseconds) {
	// Calculate the number of timer ticks needed for the given microseconds
	unsigned int timer_ticks = (microseconds * (F_CPU / 1000000UL)) / 8;
	
	// Ensure timer ticks fit within an 8-bit register (TCNT0 is 8-bit)
	if (timer_ticks > 255) {
		timer_ticks = 255; // Cap the maximum delay to 255 microseconds
	}	

	TCCR0A = 0;					// Configures Timer 0 in normal mode
	TCCR0B = (1 << CS01);		// Configures Timer 0 in normal mode
	TCNT0 = 256 - timer_ticks;	// Sets the initial timer count based on the calculated timer ticks

	while ((TIFR0 & (1 << TOV0)) == 0); // Wait for overflow flag

	TIFR0 = (1 << TOV0);     // CClears the overflow flag
	TCCR0B = 0;              // Stops Timer 0 after the delay
}