#include "timer.h"

unsigned short MY_VARIABLE_TCNT1 = 52202;
volatile uint8_t TIMEOUT_FLAG;
volatile uint8_t seconds_count = 0;
volatile uint8_t door_open = 0;
#define BUZZER_PIN	PE4

// ####### Timer 1 => 600Hz ####### //
// Para 600 Hz: TCNT1 = 65535 - (F_CPU / (600 * 2)) = 65535 - 13333 = 52202
void config_timer1_600Hz(){
	TCCR1A = 0;		// Modo Normal
	TCCR1B = 1;	// Sem Prescaler
	MY_VARIABLE_TCNT1 = 52202;
	TCNT1 = MY_VARIABLE_TCNT1;		// = 65.535 - 13 333
	TIMSK1 = (1<<0); // Ativa interrup��o por overflow TIMER 1

}

// ####### Timer 1 => 2000Hz ####### //
// Para 2000 Hz: TCNT1 = 65535 - (F_CPU / (2000 * 2)) = 65535 - 4000 = 61535
void config_timer1_2kHz(){
	TCCR1A = 0;		// Modo Normal
	TCCR1B = 1;	// Sem Prescaler
	MY_VARIABLE_TCNT1 = 61535;
	TCNT1 = MY_VARIABLE_TCNT1;		// = 65.535 - 4000
	TIMSK1 = (1<<0); // Ativa interrup��o por overflow TIMER 1

}

void init_timer1(void) {
	// Configure Timer1
	TCCR1A = 0; // Normal mode
	TCCR1B = (1 << WGM12) | (1 << CS12); // CTC mode, prescaler 256
	OCR1A = 62499; // 1 second (16MHz / 256 / 1Hz)
	TIMSK1 = (1 << OCIE1A); // Enable Timer1 compare interrupt
	sei(); // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	seconds_count++;
	if (seconds_count >= 30) {
		TIMEOUT_FLAG = 1;
	}
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

void init_interrupts(void) {
	// Configura a interrup��o externa para o pino PE5 (INT5)
	EICRB |= (1 << ISC51) | (1 << ISC50); // Configura INT5 para gerar interrup��o em qualquer mudan�a de n�vel
	EIMSK |= (1 << INT5);  // Habilita a interrup��o INT5
	sei(); // Habilita interrup��es globais
}

void init_timer3_buzzer(void) {
	// Set the buzzer pin as output
	DDRE |= (1 << BUZZER_PIN);

	// Configure Timer3 for Fast PWM mode, 16-bit
	TCCR3A |= (1 << WGM31) | (1 << COM3B1);  // Fast PWM, clear OC3B on Compare Match
	TCCR3B |= (1 << WGM32) | (1 << WGM33) | (1 << CS31); // Fast PWM, prescaler 8

	// Calculate the value for OCR3B to achieve 700 Hz
	// Timer frequency = 16 MHz / (8 * (1 + 0xFFFF)) = 1 Hz (when OCR3B = 0)
	// For 700 Hz, OCR3B = (16 MHz / (8 * 700)) - 1 = 1428 - 1 = 1427
	OCR3B = 1427; // Set duty cycle to 50%
}

void sound_alarm(void) {
	// Turn on the buzzer and keep it sounding continuously
	TCCR3A |= (1 << COM3B1); //Ativa PWM
	PORTE |= (1 << BUZZER_PIN); // Ensure the buzzer pin is set high
}


void stop_alarm(void){
	// Turn off the buzzer immediately
	TCCR3A &= ~(1 << COM3B1); //Desativa PWM
	PORTE &= ~(1 << BUZZER_PIN); //Garante que o pino do buzzer est� baixo
}

void init_timer4(void) {
	// Configure Timer4 for CTC mode
	TCCR4A = 0;
	TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40); // CTC mode, prescaler 1024
	OCR4A = 7812; // Set compare value for 2Hz (0.5 second period)
	TIMSK4 = (1 << OCIE4A); // Enable Timer4 compare interrupt
}

void init_led_porta(void) {
	// Set the LED pin as output
	DDRH |= (1 << PH3);
}

void blink_led(void) {
	// Toggle the LED pin
	PORTH ^= (1 << PH3);
}

ISR(TIMER4_COMPA_vect) {
	if (door_open) {
		blink_led();
	}
}