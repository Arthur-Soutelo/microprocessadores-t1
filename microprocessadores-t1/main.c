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
	init_timer3_buzzer();
}

int get_coins_menu(float *total_sum, char *product_price){
	char buffer_price[16];  // Buffer to hold the formatted string
	init_timer1();
	//unsigned int elapsed_time = 0;
	
	while(*total_sum <= atof(product_price)){
		//_delay_ms(1); // Wait 1 ms
		if (TIMEOUT_FLAG) {
			while(1){
				clear_display();
				write_string_line(1, "PERDEU");
			}
			return 0;
		}
		
		update_total_sum(total_sum);
		// Format the total sum as a string
		snprintf(buffer_price, sizeof(buffer_price), "Total: R$ %.2f", *total_sum);
	
		clear_display();
		write_string_line(1, "Preco: R$ ");
		write_string_LCD(product_price);
		write_string_line(2, buffer_price);
	}
	
	return 1;
	
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
	//receive_data_from_uart(buffer);
}

void get_card_number(char *card_number){
	char card_index;
	
	clear_display();
	write_string_line(1,"Digite o Cartao:");
	read_card_number(card_number);
}

int card_payment_menu(char *card_number, char *product_price){
	char card_index;
	card_index = find_card_index(card_number);
	
	// Card Found :
	if(card_index != -1){
		Card card1 = read_card_data(card_index);
		
		char balance_str[10];
		snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
		clear_display();
		write_string_line(1,"Cartao Valido");
		write_string_line(2,"Saldo:");
		write_string_LCD(balance_str);
		_delay_ms(3000);
		
		char key;
		clear_display();
		write_string_line(1,"Confirmar Compra?");
		write_string_line(2,"[*]Nao    [#]Sim");
		while(1){
			key = keypad_getkey();
			if(key=='*'){
				return 0;	// Compra NEGADA
			}
			else if (key=='#'){
				subtract_from_card_balance(card_number, atof(product_price));
					
				//card1 = read_card_data(card_index);
				//snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
				//clear_display();
				//write_string_line(1,"Compra Realizada");
				//write_string_line(2,"Saldo:");
				//write_string_LCD(balance_str);
				//turn_on_led();
				//_delay_ms(2000);
				//turn_off_led();
					//
				return 1;	// Compra CONFIRMADA
					
			}
		}
	}
}

int get_card_menu(char *product_price){
	char buy_confirmation = 0;
	char card_index;
	char card_number[CARD_NUMBER_LENGTH + 1]; // Buffer to hold the card number
	
	while(!buy_confirmation){
		clear_display();
		write_string_line(1,"Digite o Cartao:");
		read_card_number(card_number);
		
		send_choice_card(card_number);
		
		card_index = find_card_index(card_number);
		// Card Found :
		if(card_index != -1){
			Card card1 = read_card_data(card_index);
			
			char balance_str[10];
			snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
			clear_display();
			write_string_line(1,"Cartao Valido");
			write_string_line(2,"Saldo:");
			write_string_LCD(balance_str);
			_delay_ms(3000);
			char key;
			clear_display();
			write_string_line(1,"Confirmar Compra?");
			write_string_line(2,"[*]Nao    [#]Sim");
			while(!buy_confirmation){
				key = keypad_getkey();
				if(key=='*'){
					buy_confirmation = 1;
					return 0;	// Compra NEGADA
				}
				else if (key=='#'){
					subtract_from_card_balance(card_number, atof(product_price));
					Card card1;
					card1 = read_card_data(card_index);
					snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
					clear_display();
					write_string_line(1,"Compra Realizada");
					write_string_line(2,"Saldo:");
					write_string_LCD(balance_str);
					turn_on_led();
					_delay_ms(2000);
					turn_off_led();
					
					buy_confirmation = 1;
					return 1;	// Compra CONFIRMADA
					
				}
			}
		}
		else{
			clear_display();
			write_string_line(1,"Numero de Cartao");
			write_string_line(2,"Nao Encontrado");
		}
	}
}


void receive_serial_command(char *buffer, char *product_name, char *product_price, float total_sum, char *card_number) {
	switch (buffer[0]) {
		case 'A':
		switch (buffer[1]) {
			case 'P': { // Product Information
				get_name_from_buffer(buffer,product_name);
				
				if(product_name!= "Nao localizado."){
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
					char key = keypad_getkey();
					while(key!='1' && key!='2'){
						key = keypad_getkey();
						// Dinheiro
						if(key == '1'){
							char result;
							result = get_coins_menu(&total_sum, product_price);
							if(result == 1){
								send_confirm_cash_purchase();
								break;
							}
							else{
								clear_display();
								write_string_line(1,"Tempo Excedido");
								break;
							}
							
						}
						// Cartão
						else if (key == '2'){
							get_card_number(card_number);
							char result;
							result = card_payment_menu(card_number, product_price);
							if(result == 1){
								send_confirm_card_purchase(card_number);
								break;
							}
							else{
								clear_display();
								write_string_line(1,"Compra Cancelada");
								break;
							}
						}
					}
				}
	
			} break;

			case 'E': { // Purchase Result - Cash
				switch (buffer[2]) {// Response
				// '0' - Compra efetivada com sucesso
					case '0':
						clear_display();
						write_string_line(1,"Compra Realizada");
						turn_on_led();
						_delay_ms(2000);
						turn_off_led();
					break;				// '1' - Compra com falha (produto inválido)
					case '1':
						clear_display();
						write_string_line(1,"Compra com falha");
						write_string_line(2, "PRODUTO INVALIDO");
					break;
				// '2' - Compra com falha (quantidade insuficiente)
					case '2':
						clear_display();
						write_string_line(1,"Compra com falha");
						write_string_line(2, "QTD INSUFICIENTE");
					break;
				// '3' - Compra com falha (validade vencida)
					case '3':
						clear_display();
						write_string_line(1,"Compra com falha");
						write_string_line(2, "VALIDADE VENCIDA");
					break;
				}
			} break;

			case 'C': { // Purchase Result - Card
				switch (buffer[2]) {// Response
					// '0' - Compra efetivada com sucesso
					case '0':{
						char card_index;
						char balance_str[10];
						Card card1;
						
						card_index = find_card_index(card_number);
						card1 = read_card_data(card_index);
						snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
						
						clear_display();
						write_string_line(1,"Compra Realizada");
						write_string_line(2,"Saldo:");
						write_string_LCD(balance_str);
						turn_on_led();
						_delay_ms(2000);
						turn_off_led();
					}break;					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1,"Compra com falha");
					write_string_line(2, "PRODUTO INVALIDO");
					break;
					// '2' - Compra com falha (quantidade insuficiente)
					case '2':
					clear_display();
					write_string_line(1,"Compra com falha");
					write_string_line(2, "QTD INSUFICIENTE");
					break;
					// '3' - Compra com falha (validade vencida)
					case '3':
					clear_display();
					write_string_line(1,"Compra com falha");
					write_string_line(2, "VALIDADE VENCIDA");
					break;
					// '4' - Compra com falha (cartão inválido)
					case '4':
					clear_display();
					write_string_line(1,"Compra com falha");
					write_string_line(2, "CARTAO INVALIDO");
					break;
				}
			} break;

			case 'Q': { // Quantity - Result
				buffer[2] = uart_receive(); // Response
				// '0' - Compra efetivada com sucesso				// '1' - Compra com falha (produto inválido)
				// '2' - Compra com falha (quantidade inválida)
			} break;
			
			case 'R': { // Cash Removal - Result
			} break;
			
			case 'A': { // Update Card - Result
				buffer[2] = uart_receive(); // Response
				// '0' - Atualização efetivada com sucesso				// '1' - Atualização com falha (valor inválido)
				// '4' - Atualização com falha (cartão inválido)
			} break;

			case 'I': { // Add Card - Result
			} break;

			default:
			// Handle unexpected input
			break;
		}
		break;

		default:
		// Handle unexpected input
		break;
	}
}

int main(void){
	static float total_sum = 0.0;	// Define total sum variable
	char buffer[BUFFER_SIZE];		// Buffer to hold the uart response
	char product_name[NAME_SIZE];
	char product_price[NAME_SIZE];
	char card_number[CARD_NUMBER_LENGTH + 1]; // Buffer to hold the card number
	char key;
	char go_back_flag;
	init_components();
	//init_base_cards();
	//sei();			// Ativa interrupt
		
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
			
			//if (uart_ready()){
				//receive_data_from_uart(buffer);
				//receive_serial_command(buffer, product_name, product_price, total_sum,card_number);
			//}

			total_sum = 0.0;
			key = keypad_getkey();
			// Seleção do produto pelo codigo
			if(key!=0){
				get_selected_product_menu(key, buffer);
				// Se resposta valida
				
				while(!go_back_flag){
					receive_data_from_uart(buffer);
					receive_serial_command(buffer, product_name, product_price, total_sum,card_number);
					
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