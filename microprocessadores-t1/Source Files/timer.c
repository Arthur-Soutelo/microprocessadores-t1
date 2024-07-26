#include "timer.h"

unsigned short MY_VARIABLE_TCNT1 = 52202;
volatile uint8_t TIMEOUT_FLAG;
volatile uint8_t seconds_count = 0;
volatile uint16_t elapsed_time_2 = 0; // Track elapsed time


// ================================= Timer 1 ================================= //
// ========================== TIMEOUT INSERT COINS ========================== //


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


void reset_timer_1(void) {
	seconds_count = 0;
}


// ================================= Timer 2  ================================= //
// ========================= OPERATOR LOGIN TIMEOUT ========================= //

// login timeout
void timer2_init(void) {
	// Set Timer2 to CTC mode
	TCCR2A = (1 << WGM21);
	// Set prescaler to 64 and start Timer2
	TCCR2B = (1 << CS22);
	// Set compare match value for 1ms interrupt
	OCR2A = 249; // Assuming 16MHz clock, prescaler 64, results in 1ms period
	// Enable Timer2 compare match A interrupt
	TIMSK2 = (1 << OCIE2A);
	// Enable global interrupts
	sei();
}

void reset_timer_2(void) {
	elapsed_time_2 = 0;
}

char is_timeout_2(void) {
	return (elapsed_time_2 >= 12000); // Timeout period in milliseconds
}

void stop_timer2(void) {
	// Clear the prescaler bits in TCCR2B to stop the timer
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}

// ================================= Timer 3  ================================= //
// ================================= BUZZER ================================= //

void init_door_buzzer(void) {
	 // Set PE4 as output
	 DDRE |= (1 << PE4);
	 // Set CIRCUIT_PIN as input
	 DDRE &= ~(1 << DOOR_PIN);
	 // Enable pull-up resistor on CIRCUIT_PIN
	 PORTE |= (1 << DOOR_PIN);
	 
	 // Configure Timer 3 for 700 Hz
	 TCCR3A = 0; // Normal mode
	 TCCR3B = (1 << WGM32) | (1 << CS31); // CTC mode, prescaler 8
	 OCR3A = (F_CPU / (2 * 8 * 700)) - 1; // Set compare value for 700 Hz

	 // Enable Timer 3 compare interrupt
	 TIMSK3 |= (1 << OCIE3A);
	 
	 // Enable external interrupt on INT5
	 //EICRB |= (1 << ISC51) | (1 << ISC50); // Configura INT5 para gerar interrupção em qualquer mudança de nível
	 EICRB |= (1 << ISC51);  // Trigger on falling edge
	 EIMSK |= (1 << INT5);   // Enable INT5
}

void sound_alarm(void) {
	// Start Timer 3
	TCCR3B |= (1 << CS31); // Start Timer3 with prescaler 8
}


void stop_alarm(void){
	// Stop Timer 3 and turn off the buzzer
	TCCR3B &= ~(1 << CS31); // Stop Timer3 by clearing the clock source
	PORTE &= ~(1 << PE4); // Ensure buzzer is turned off
}

// ================================= Timer 4  ================================= //
// ================================ DOOR LED ================================ //

void init_timer4(void) {
	// Configure Timer4 for CTC mode
	TCCR4A = 0;
	TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40); // CTC mode, prescaler 1024
	OCR4A = (F_CPU / (1024 * 4)) - 1; // Set compare value for 2 Hz
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
