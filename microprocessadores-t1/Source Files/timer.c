#include "timer.h"

unsigned short MY_VARIABLE_TCNT1 = 52202; // Valor inicial do contador do Timer1
volatile uint8_t TIMEOUT_FLAG;
volatile uint8_t seconds_count = 0; // Contador de segundos para o Timer1
volatile uint16_t elapsed_time_2 = 0; // Contador de tempo decorrido para o Timer2


// ================================= Timer 1 ================================= //
// ========================== TIMEOUT INSERT COINS ========================== //


void init_timer1(void) {
	// Configure Timer1
	TCCR1A = 0; // Normal mode
	TCCR1B = (1 << WGM12) | (1 << CS12); // Modo CTC, prescaler 256
	OCR1A = 62499; // 1 segundo (16MHz / 256 / 1Hz)
	TIMSK1 = (1 << OCIE1A); // Habilita a interrupção de comparação do Timer1
	sei(); // Habilita interrupções globais
}

ISR(TIMER1_COMPA_vect) {
	seconds_count++;
	if (seconds_count >= 30) {
		TIMEOUT_FLAG = 1; // Define a flag de timeout após 30 segundos utilizada para tempo máximo de inserção de moedas
	}
}

// Parar Timer1
void timer1_stop(void) {
	
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
	PORTB &= ~(1<<PB5);// Garantir que o pino PB5 esta em nivel baixo
}

// Reseta o contador de segundos e a flag de timeout
void reset_timer_1(void) {
	seconds_count = 0;
	TIMEOUT_FLAG = 0;
}


// ================================= Timer 2  ================================= //
// ========================= OPERATOR LOGIN TIMEOUT ========================= //

// login timeout
void timer2_init(void) {
	TCCR2A = (1 << WGM21);   // Configura o Timer2 para o modo CTC
	TCCR2B = (1 << CS22);	 // Set prescaler to 64 and start Timer2
	// Configura o valor de comparação para interrupção de 1ms	
	OCR2A = 249;			 // Assumindo clock de 16MHz, prescaler 64, resulta em período de 1ms
	// Habilita a interrupção de comparação do Timer2
	TIMSK2 = (1 << OCIE2A);
	sei();					 // Habilita interrupções globais
}

// Reseta o tempo decorrido para o Timer2
void reset_timer_2(void) {
	elapsed_time_2 = 0;
}

// Verifica se o tempo decorrido é maior ou igual a 12000ms (12 segundos), utilizada na verificação de porta aberta
char is_timeout_2(void) {
	return (elapsed_time_2 >= 12000);
}

// Para o Timer2 limpando os bits do prescaler no TCCR2B
void stop_timer2(void) {
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}

// ================================= Timer 3  ================================= //
// ================================= BUZZER ================================= //

// Configura timer utilizado no controle do buzzer de porta aberta
void init_door_buzzer(void) {
	 
	 DDRE |= (1 << PE4);	    // Configura o pino PE4 como saída
	 DDRE &= ~(1 << DOOR_PIN);  // Configura o pino DOOR_PIN como entrada
	 PORTE |= (1 << DOOR_PIN);  // Habilita o resistor de pull-up no pino DOOR_PIN
	 
     // Configura o Timer 3 para 700 Hz
	 TCCR3A = 0;						  // Modo Normal
	 TCCR3B = (1 << WGM32) | (1 << CS31); // Modo CTC, prescaler 8
	 OCR3A = (F_CPU / (2 * 8 * 700)) - 1; // Configura valor de comparação para 700 Hz
	
	 // Habilita a interrupção de comparação do Timer 3
	 TIMSK3 |= (1 << OCIE3A);
	 
	 // Habilita a interrupção externa no INT5
	 EICRB |= (1 << ISC51);  // Gera interrupção na borda de descida
	 EIMSK |= (1 << INT5);   // Habilita INT5
}

void sound_alarm(void) {
	TCCR3B |= (1 << CS31); // Inicia Timer3 com prescaler 8
}

void stop_alarm(void){
	TCCR3B &= ~(1 << CS31); // Para o Timer3 limpando a fonte de clock
	PORTE &= ~(1 << PE4);   // Garante que o buzzer está desligado
}

// ================================= Timer 4  ================================= //
// ================================ DOOR LED ================================ //

void init_timer4(void) {
	TCCR4A = 0;
	TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40); // Modo CTC, prescaler 1024
	OCR4A = (F_CPU / (1024 * 4)) - 1;				   // Configura valor de comparação para 2 Hz
	TIMSK4 = (1 << OCIE4A);                            // Habilita a interrupção de comparação do Timer4
}

void init_led_porta(void) {
	DDRH |= (1 << PH3);   // Set the LED pin as output
}

void blink_led(void) {
	PORTH ^= (1 << PH3);  // Alterna o estado do pino do LED
}
