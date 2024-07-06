// Projeto 1- Microprocessadores	-	Turma : B	-	2024
//Authors: Arthur Antonio Soutelo Araujo - 00304292 e Gabriel Rosado dos Santos Mendes - 00301564

#include "main_header.h"


// Interrupt Timer 1	-	16bits
ISR(TIMER1_OVF_vect){
	TCNT1 = 52202;	//600
	PORTB ^= (1<<PORTB5);
	TIFR1 = (1<<0);
}

int main(void){
    // Inicializar UART
    uart_init();
    
    DDRB = (1<<PORTB5);	// Ativa como saida PB5 (13)
    
    sei();		// Ativa interrupt

    unsigned char received_char;
    
    while (1) {
	    received_char = uart_receive();
	    
    }
}