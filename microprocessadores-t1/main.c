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
	// Escreve uma mensagem no LCD
	write_string_line(1,"VenDELET");
	write_string_line(2,"Fais ton choix");

	// Inicializa o Teclado
	keypad_init();

	// Inicializa Serial
    uart_init();
    //sei();			// Ativa interrupt
	
	char key;
    while(1){
		// Get the pressed key
		key = keypad_getkey();
		
		if(key!=0){
			clear_display();
			write_string_line(1,"Boa ");
			//write_string_line(2,key);
			write_data_LCD(key);
			uart_send(key);
			
			_delay_ms(300);
			//write_string_LCD(key);
		}
	}
}