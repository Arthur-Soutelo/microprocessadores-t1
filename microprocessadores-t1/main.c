// Tabalho 1- Microprocessadores	-	Turma : B	-	2024
// Author: Arthur Antonio Soutelo Araujo	- 00304292
// Author: Gabriel Rosado dos Santos Mendes - 00301564

#include "main_header.h"

// Interrupt Timer 1	-	16bits
ISR(TIMER1_OVF_vect){
	TCNT1 = 52202;	//600
	PORTB ^= (1<<PORTB5);
	TIFR1 = (1<<0);
}

int main(void){

    uart_init();	// Inicializar UART
    sei();			// Ativa interrupt
    
    while(1){
	    
    }
}