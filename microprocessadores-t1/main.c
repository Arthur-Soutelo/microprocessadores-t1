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

void init_components(void){
	init_LCD();			// Inicializa o LCD
	keypad_init();		// Inicializa o Teclado
	uart_init(19200);	// Initialize the UART with desired baud rate
	buttons_init();		// Initialize coins reading
	door_init();		// Initialize door sensor reading
	init_buzzer();
}

void get_coins_menu(float *total_sum, char *product_price){
	char buffer_price[16];  // Buffer to hold the formatted string
	init_timer1();
	//unsigned int elapsed_time = 0;
	
	while(*total_sum <= atof(product_price)){
		//_delay_ms(1); // Wait 1 ms
		if (timeout_flag) {
			while(1){
				clear_display();
				write_string_line(1, "PERDEU");
			}
			return;
		}
		
		update_total_sum(total_sum);
		// Format the total sum as a string
		snprintf(buffer_price, sizeof(buffer_price), "Total: R$ %.2f", *total_sum);
	
		clear_display();
		write_string_line(1, "Preco: R$ ");
		write_string_LCD(product_price);
		write_string_line(2, buffer_price);
	}
	clear_display();
	write_string_line(1, "COMPRA REALIZADA");
	turn_on_led();
	
	_delay_ms(2000);
	
	turn_off_led();
	
}

void get_selected_product_menu(char key, char *buffer){
	// Get the pressed key
	clear_display();
	write_string_line(1,"VenDELET");
	write_string_line(2,"Produto : ");
	ProductNumber product = get_product_number(key);
	write_data_LCD(product.first_key);
	write_data_LCD(product.second_key);
	
	// Envia produto
	send_product_selection(product);
	// Recebe resposta do produto
	receive_data_from_uart(buffer);
}

void get_card_menu(void){
	char buy_confirmation = 0;
	char card_index;
	char card_number[CARD_NUMBER_LENGTH + 1]; // Buffer to hold the card number

	
	const char* search_card_number = "300123";	// Example card number to search for
	
	while(!buy_confirmation){
		clear_display();
		write_string_line(1,"Digite o Cartao:");
		read_card_number(card_number);
		card_index = find_card_index(card_number);
		// Card Found :
		if(card_index != -1){
			Card card1 = read_card_data(card_index);
			
			
			char balance_str[10];
			snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
			clear_display();
			write_string_line(1,"Saldo:");
			write_string_LCD(balance_str);
			while(!buy_confirmation){
				
			}
		}
		else{
			clear_display();
			write_string_line(1,"Numero de Cartao");
			write_string_line(2,"Nao Encontrado");
		}
	}
}

int main(void){
	static float total_sum = 0.0;	// Define total sum variable
	char buffer[BUFFER_SIZE];		// Buffer to hold the uart response
	char product_name[NAME_SIZE];
	char product_price[NAME_SIZE];
	char key;
	char go_back_flag;
	init_components();
	

	init_base_cards();

    //sei();			// Ativa interrupt
	
	//while(1){
		////unsigned char rec; 
		////rec = uart_receive();
		////if(rec != 0xFF){
			////uart_send(rec);
			////clear_display();
			////write_string_line(1,buffer);
		////}
		//
		//receive_data_from_uart(buffer);
		//clear_display();
		//write_string_line(1,buffer);
		//uart_send_string(buffer);
		//
	//}
	
    while(1){
		go_back_flag = 0;
		stop_alarm();
		clear_display();
		write_string_line(1,"VenDELET");
		write_string_line(2,"Digite o Produto");
		
		while(!read_door_state()){	// While the door is closed
			if (go_back_flag == 1) {
				break; // Exit the product selection loop and the outer loop
			}
			
			key = keypad_getkey();
			// Seleção do produto pelo codigo
			if(key!=0){
				get_selected_product_menu(key, buffer);
				
				// Se resposta valida
				if(buffer[0] == 'A' && buffer[1] == 'P' ){
					get_name_from_buffer(buffer,product_name);
					get_price_from_buffer(buffer,product_price);
					
					// Escreve o produto e preço no LCD
					clear_display();
					write_string_line(1,product_name);
					write_string_line(2, "Valor: R$ ");
					write_string_LCD(product_price);
					_delay_ms(3000);
				
					// Seleciona método de pagamento :
					clear_display();
					write_string_line(1,"1 - Dinheiro");
					write_string_line(2, "2 - Cartao");
					key = keypad_getkey();
					while(key==0){
						if (go_back_flag == 1) {
							break; // Exit the product selection loop and the outer loop
						}
						key = keypad_getkey();
						//_delay_ms(50);
						// Dinheiro
						if(key == '1'){
							// Wait for payment
							get_coins_menu(&total_sum, product_price);

						}
						// Cartão
						else if (key == '2'){
							go_back_flag = 1;
							get_card_menu();
						}
					}
				}
			}
				
		}
				
		
		while(read_door_state()){ // While the door is open
			// Sound the alarm
			sound_alarm();
			// Door is open
			clear_display();
			write_string_line(1,"Porta Aberta");
		}
		
	}
	return 0;
}