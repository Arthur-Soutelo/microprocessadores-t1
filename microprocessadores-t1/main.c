// Tabalho 1- Microprocessadores	-	Turma : B	-	2024
// ATmega2560 //
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

    //uart_init();	// Inicializar UART
    //sei();			// Ativa interrupt
    
	init_LCD();
	//write_data_LCD('a');
	//clear_display();
	//write_command_LCD(0x80); // Set cursor to start of line 1
	//write_data_LCD('a');
	
	char message[] = "LCD Test";
	write_string(1,message);
	delay_ms(1000);
	write_string(2,message);
	delay_ms(1000);
	
	clear_display();
	
    while(1){
	
    }
}