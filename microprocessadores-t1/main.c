// Tabalho 1- Microprocessadores	-	Turma : B	-	2024
// ATmega2560 //
// Author: Arthur Antonio Soutelo Araujo	- 00304292
// Author: Gabriel Rosado dos Santos Mendes - 00301564

#include "main_header.h"

static float total_sum = 0.0;	// Define total sum variable
unsigned char buffer[BUFFER_SIZE];		// Buffer to hold the uart response
char product_name[NAME_SIZE];
char product_price[NAME_SIZE];
char card_number[CARD_NUMBER_LENGTH + 1]; // Buffer to hold the card number
char key;

// UART RECIVE INTERRUPT
unsigned char buffer_index = 0;
ISR(USART0_RX_vect) {
	char receivedChar = UDR0; // Leia o caractere recebido
	buffer[buffer_index] = receivedChar;
	
	buffer_index++;
	if(buffer_index >= 4){
		if(buffer[0]=='A' && buffer[1]=='P'){
			unsigned char message_size = buffer[2];
			if(message_size == buffer_index-3){
				buffer_index = 0;
				analyze_serial_command(buffer, product_name, product_price, total_sum,card_number);
			}
		}
		else if (buffer_index == 4){
			buffer_index = 0;
			analyze_serial_command(buffer, product_name, product_price, total_sum,card_number);
		}
	}
}

ISR(TIMER3_COMPA_vect) {
	// Toggle the buzzer pin on each compare match interrupt
	PORTE ^= (1 << PE4);
}

ISR(INT5_vect){
	
	if (read_door_state()) { // Verifica o estado da porta
		stop_alarm(); // Para o buzzer se estiver tocando

		} else{
		sound_alarm(); // Toca o buzzer
		clear_display();
		write_string_line(1, "Porta Aberta");
	}
}

void init_components(void){
	init_LCD();			// Inicializa o LCD
	keypad_init();		// Inicializa o Teclado
	uart_init(19200);	// Initialize the UART with desired baud rate
	buttons_init();		// Initialize coins reading
	door_init();		// Initialize door sensor reading
	//init_timer3_buzzer();
	init_base_cards();
	init_operator();
	UCSR0B |= (1 << RXCIE0); // Ativa a interrupção de recepção
	sei(); // Habilita as interrupções globais
 }

int get_coins_menu(float *total_sum, const char *product_price){
	char buffer_price[16];  // Buffer to hold the formatted string
	init_timer1();
	
	while(*total_sum <= atof(product_price)){
		if (TIMEOUT_FLAG) {
			return 0;
		}
		
		update_total_sum(total_sum);
		// Format the total sum as a string
		snprintf(buffer_price, sizeof(buffer_price), "Total: R$ %.2f", *total_sum);
	
		clear_display();
		write_string_line(1, "Preco: R$ ");
		write_string_LCD(product_price);
		write_string_line(2, buffer_price);
		
		if(*total_sum == atof(product_price)){
			return 1;
		}
	}
	
	return 0;
	
}

void get_selected_product_menu(char key){
	// Get the pressed key
	clear_display();
	write_string_line(1,"VenDELET");
	write_string_line(2,"Produto : ");
	ProductNumber product = get_product_number(key);
	write_data_LCD(product.first_key);
	write_data_LCD(product.second_key);
	
	// Envia produto
	send_product_selection(product);
}

void get_card_number(char *card_number){
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
				return 1;	// Compra CONFIRMADA
					
			}
		}
	}
}


int operator_login(void){
	clear_display();
	write_string_line(1, "Modo Operador");
	_delay_ms(3000);
	
	char login[LOGIN_SIZE]; // Ajuste o tamanho conforme necessário
	read_login(login);
	
	// Define the source string
	char login_line[16] = "User: ";
	// Concatenate the source string to the destination string
	strncat(login_line, login, LOGIN_SIZE);
	
	char pwd[PASSWORD_SIZE]; // Ajuste o tamanho conforme necessário
	read_pwd(login_line, pwd);
	
	
	uart_send_string(login);
	uart_send_string(pwd);
	
	clear_display();
	_delay_ms(5);
	write_string_line(1, login);
	write_string_line(2, pwd);
	while(1);
	
	return validate_user(login, pwd);
}

// Função para obter a entrada do operador do menu
void get_menu_operator(void) {
	const char *options[] = {
		"Adicionar Cartao",
		"Remover Cartao",
		"Abastecer",
		"Retirar Caixa",
		"Sair"
	};
	int num_options = sizeof(options) / sizeof(options[0]);

	int current_option = navigate_options(options, num_options);

	// Chama a função correspondente à opção selecionada
	switch (current_option) {
		// Adicionar Cartao
		case 0: {
			char card_number[16]; // Ajuste o tamanho conforme necessário
			read_card_number(card_number);
			add_new_card(card_number, 0.00);
		} break;
		// Remover Cartao
		case 1: {
			char card_number[16]; // Ajuste o tamanho conforme necessário
			read_card_number(card_number);
			remove_card(card_number);
		} break;
		case 2: {
			// Adicione a lógica para o caso '3'
		} break;
		case 3: {
			// Adicione a lógica para o caso '4'
		} break;
		default: {
			// Opcional: Lógica para tecla inválida
		} break;
	}
}

void analyze_serial_command(unsigned char *buffer, char *product_name, char *product_price, float total_sum, char *card_number) {
	switch (buffer[0]) {
		case 'A':
		switch (buffer[1]) {
			case 'P': { // Product Information
				get_name_from_buffer(buffer, product_name);
				
				// Verifica se a string é igual
				if (strcmp(product_name, "Nao localizado.") == 0) {
					get_price_from_buffer(buffer, product_price);
					
					// Escreve o produto e preço no LCD
					clear_display();
					write_string_line(1, product_name);
					write_string_line(2, "Valor: R$ ");
					write_string_LCD(product_price);
					_delay_ms(3000);
					
					// Seleciona método de pagamento
					clear_display();
					write_string_line(1, "1 - Dinheiro");
					write_string_line(2, "2 - Cartao");
					char key = keypad_getkey();
					while (key != '1' && key != '2') {
						key = keypad_getkey();
					}
					// Compra por Dinheiro
					if (key == '1') {
						char result = get_coins_menu(&total_sum, product_price);
						if (result == 1) {
							send_confirm_cash_purchase();
							} else {
							clear_display();
							write_string_line(1, "Tempo Excedido");
						}
					}
					// Compra por Cartão
					else if (key == '2') {
						get_card_number(card_number);
						char result = card_payment_menu(card_number, product_price);
						if (result == 1) {
							send_confirm_card_purchase(card_number);
							} else {
							clear_display();
							write_string_line(1, "Compra Cancelada");
						}
					}
					} else {
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "NAO LOCALIZADO");
				}
			} break;

			case 'E': { // Purchase Result - Cash
				switch (buffer[2]) {
					// '0' - Compra efetivada com sucesso
					case '0':
					clear_display();
					write_string_line(1, "Compra Realizada");
					turn_on_led();
					_delay_ms(3000);
					turn_off_led();
					break;
					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "PRODUTO INVALIDO");
					break;
					// '2' - Compra com falha (quantidade insuficiente)
					case '2':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "QTD INSUFICIENTE");
					break;
					// '3' - Compra com falha (validade vencida)
					case '3':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "VALIDADE VENCIDA");
					break;
				}
			} break;

			case 'C': { // Purchase Result - Card
				switch (buffer[2]) {
					// '0' - Compra efetivada com sucesso
					case '0': {
						char card_index;
						char balance_str[10];
						Card card1;
						
						card_index = find_card_index(card_number);
						card1 = read_card_data(card_index);
						snprintf(balance_str, sizeof(balance_str), "%.2f", card1.balance);
						
						clear_display();
						write_string_line(1, "Compra Realizada");
						write_string_line(2, "Saldo:");
						write_string_LCD(balance_str);
						turn_on_led();
						_delay_ms(3000);
						turn_off_led();
					} break;
					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "PRODUTO INVALIDO");
					break;
					// '2' - Compra com falha (quantidade insuficiente)
					case '2':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "QTD INSUFICIENTE");
					break;
					// '3' - Compra com falha (validade vencida)
					case '3':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "VALIDADE VENCIDA");
					break;
					// '4' - Compra com falha (cartão inválido)
					case '4':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "CARTAO INVALIDO");
					break;
				}
			} break;

			case 'Q': { // Quantity - Result
				switch (buffer[2]) {
					// '0' - Compra efetivada com sucesso
					case '0':
					// CODE HERE
					break;
					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "PRODUTO INVALIDO");
					break;
					// '2' - Compra com falha (quantidade inválida)
					case '2':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "QTD INVALIDA");
					break;
				}
			} break;

			case 'R': { // Cash Removal - Result
				clear_display();
				write_string_line(1, "Dinheiro Removido");
			} break;

			case 'A': { // Update Card - Result
				switch (buffer[2]) {
					// '0' - Atualização efetivada com sucesso
					case '0':
					// CODE HERE
					break;
					// '1' - Atualização com falha (valor inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "VALOR INVALIDO");
					break;
					// '4' - Atualização com falha (cartão inválido)
					case '4':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "CARTAO INVALIDO");
					break;
				}
			} break;

			case 'I': { // Add Card - Result
				clear_display();
				write_string_line(1, "Cartao Inserido");
			} break;
		}
		break;
	}
}

int main(void){
	init_components();
	
	//get_menu_operator();	
		
		stop_alarm();
		clear_display();
		write_string_line(1,"VenDELET");
		write_string_line(2,"Digite o Produto");
	while(1){	
		//while(!read_door_state()){	// While the door is closed
			total_sum = 0.0;
			key = keypad_getkey();
			// Seleção do produto pelo codigo
			if(key!=0){
				if(key=='D'){
					if (operator_login()==1){
						get_menu_operator();	
					}
					else{
						clear_display();
						write_string_line(1, "----- ERRO -----");
						write_string_line(2, " LOGIN INVALIDO");
					}
					
				}
				else{
					get_selected_product_menu(key);	
				}
			}
			
		//}
		
		
		//while(read_door_state()){ // While the door is open
			//// Sound the alarm
			//sound_alarm();
			//// Door is open
			//clear_display();
			//write_string_line(1,"Porta Aberta");
		//}


	}
	
	return 0;
}