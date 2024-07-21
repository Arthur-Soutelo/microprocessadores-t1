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
	uart_init(9600);	// Initialize the UART with desired baud rate
	buttons_init();		// Initialize coins reading
	door_init();		// Initialize door sensor reading
	init_buzzer();
}

void get_coins_menu(float *total_sum, char *product_price){
	char buffer_price[16];  // Buffer to hold the formatted string
	
	update_total_sum(total_sum);
	// Format the total sum as a string
	snprintf(buffer_price, sizeof(buffer_price), "Total: R$%.2f", *total_sum);
	
	clear_display();
	write_string_line(1, "Preco: R$");
	write_string_LCD(product_price);
	write_string_line(2, buffer_price);

	//
	//// Get coins
	//if(button50c_clicked()){
		//clear_display();
		//write_string_line(1,"Button 50c");
		//// Add a small delay to avoid multiple detections of the same click
		////_delay_ms(5);
	//}
	//if(button1r_clicked()){
		//clear_display();
		//write_string_line(1,"Button 1R");
		//// Add a small delay to avoid multiple detections of the same click
		////_delay_ms(5);
	//}
}

char get_selected_product_menu(void){
	// Get the pressed key
	char key;
	key = keypad_getkey();
	if(key!=0){
		clear_display();
		write_string_line(1,"VenDELET");
		write_string_line(2,"Produto : ");
		ProductNumber product = get_product_number(key);
		send_product_selection(product);
		
		write_data_LCD(product.first_key);
		write_data_LCD(product.second_key);
		
		return 1;
	}
	return 0;
}

int main(void){
	static float total_sum = 0.0;	// Define total sum variable
	char buffer[BUFFER_SIZE];		// Buffer to hold the uart response
	char product_name[NAME_SIZE];
	char product_price[NAME_SIZE];
	char payment_method;
	
	init_components();
	
    //sei();			// Ativa interrupt

	clear_display();	
	write_string_line(1,"VenDELET");
	write_string_line(2,"Digite o Produto");
	
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
		stop_alarm();
		while(!read_door_state()){	// While the door is closed
			
			// Seleção do produto pelo codigo
			if(get_selected_product_menu()){
				
				// Recebe resposta do produto
				receive_data_from_uart(buffer);
				get_name_from_buffer(buffer,product_name);
				get_price_from_buffer(buffer,product_price);
				
				// Se resposta valida
				if(buffer[0] == 'A' && buffer[1] == 'P' ){
					// Escreve o produto e preço no LCD
					clear_display();
					write_string_line(1,product_name);
					write_string_line(2, "Valor: R$");
					write_string_LCD(product_price);
					_delay_ms(3000);
				
					// Seleciona método de pagamento :
					clear_display();
					write_string_line(1,"1 - Dinheiro");
					write_string_line(2, "2 - Cartao");
					payment_method = keypad_getkey();
					while(payment_method==0){
						payment_method = keypad_getkey();
						//_delay_ms(50);
						// Dinheiro
						if(payment_method == '1'){
							// Wait for payment
							while(total_sum <= atof(product_price)){
								get_coins_menu(&total_sum, product_price);
							}
						}
						// Cartão
						else if (payment_method == '2'){
							//get_card_menu();
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