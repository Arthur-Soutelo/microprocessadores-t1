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

void get_coins_menu(float *total_sum){
	//char sum
	//sum = update_total_sum()
	//
	//char buffer[16];  // Buffer to hold the formatted string
	//// Format the total sum as a string
	//snprintf(buffer, sizeof(buffer), "Total: $%.2f", total_sum);
	
	// Get coins
	if(button50c_clicked()){
		clear_display();
		write_string_line(1,"Button 50c");
		// Add a small delay to avoid multiple detections of the same click
		//_delay_ms(5);
	}
	if(button1r_clicked()){
		clear_display();
		write_string_line(1,"Button 1R");
		// Add a small delay to avoid multiple detections of the same click
		//_delay_ms(5);
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
		ProductNumber product = get_product_number(key);
		// Use the product number (first_key and second_key)
		send_product_selection(ProductNumber);
		
		write_data_LCD(product.first_key);
		write_data_LCD(product.second_key);
		
		//write_data_LCD(key);
		//send_product_number(key);
		_delay_ms(1000);
	}
}

int main(void){
	// Define total sum variable
	static float total_sum = 0.0;
	
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
	
	init_buzzer();
	
    //sei();			// Ativa interrupt

	clear_display();	
	write_string_line(1,"VenDELET");
	write_string_line(2,"Digite o Produto");
	
    while(1){
		stop_alarm();
		while(!read_door_state()){	// While the door is closed
			
			get_selected_product_menu();
			
			// Get coins
			//get_coins_menu(&total_sum);
		
		}
		while(read_door_state()){
			// Sound the alarm
			sound_alarm();
			// Door is open
			clear_display();
			write_string_line(1,"Porta Aberta");
		}
		
	}
}