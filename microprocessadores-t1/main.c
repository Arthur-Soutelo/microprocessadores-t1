// Tabalho 1- Microprocessadores	-	Turma : B	-	2024
// ATmega2560 //
// Author: Arthur Antonio Soutelo Araujo	- 00304292
// Author: Gabriel Rosado dos Santos Mendes - 00301564

#include "main_header.h"

// Interrupt Timer 1	-	16bits
//ISR(TIMER1_OVF_vect){
	//TCNT1 = 52202;	//600
	//PORTB ^= (1<<PORTB5);
	//TIFR1 = (1<<0);
//}


int main(void){
	// Inicializa o LCD
	init_LCD();
//
	//// Escreve uma mensagem no LCD
	//write_string_LCD("Hello, MENDES");
	write_string_line(1,"VenDELET");
	write_string_line(2,"Fais ton choix");

	keypad_init();

    //uart_init();	// Inicializar UART
    //sei();			// Ativa interrupt
	
	
    while(1){
		char key;
		key = keypad_getkey();
		if(key!=0){
			clear_display();
			write_string_line(1,"Boa");
			write_string_line(2,"XXXXX");
		}
	}
}