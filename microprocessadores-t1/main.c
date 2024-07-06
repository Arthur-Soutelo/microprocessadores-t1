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
	    uart_send(received_char);
	    switch (received_char) {
		    case '1':
				config_timer_600Hz();
				uart_send('A'); // Envia 'A'
		    break;
		    case '2':
				config_timer_2kHz();
				uart_send('B'); // Envia 'B'
		    break;
		    case '3':
				timer1_stop();
				uart_send('C'); // Envia 'C'
		    break;
		    default:
		    break;
	    }
    }
}