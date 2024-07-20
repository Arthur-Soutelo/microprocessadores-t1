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

void get_coins_menu(void){
	// Get coins
	if(button1_clicked()){
		clear_display();
		write_string_line(1,"Button 1");
		// Add a small delay to avoid multiple detections of the same click
		_delay_ms(300);
	}
	if(button2_clicked()){
		clear_display();
		write_string_line(1,"Button 2");
		// Add a small delay to avoid multiple detections of the same click
		_delay_ms(300);
	}
}

void get_selected_product_menu(void){
	// Get the pressed key
	char key;
	key = keypad_getkey();
	if(key!=0){
		clear_display();
		write_string_line(1,"Selecione o Produto:");
		write_string_line(2,"Numero: ");
		write_data_LCD(key);
		send_product_number(key);
		_delay_ms(300);
	}
}

int main(void){
	// Inicializa o LCD
	init_LCD();

	// Inicializa o Teclado
	keypad_init();
	// Initialize the UART with desired baud rate
	uart_init(9600);
	// Initialize coins reading
	buttons_init();
	// Initialize door sensor reading
	door_init();
	
    //sei();			// Ativa interrupt
	
    while(1){
		while(read_door_state()){	// While the door is closed
			clear_display();
			write_string_line(1,"VenDELET");
			write_string_line(2,"Digite o Produto");
			
			get_selected_product_menu();
			
			// Get coins
			get_coins_menu();
		}	
		// Door is open
		clear_display();
		write_string_line(1,"Porta Aberta");
		_delay_ms(300);
		
	}
}