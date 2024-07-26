/*****************************************************************************/
/*					UNIVERSIDADE FEDERAL DO RIO GRANDE DO SUL				 */
/*							   ESCOLA DE ENGENHARIA						     */
/*					   DEPARTAMENTO DE ENGENHARIA ELÉTRICA					 */
/*					CURSO DE GRADUAÇÃO EM ENGENHARIA ELETRICA				 */
/*						ENG04475 - MICROPROCESSADORES I						 */
/*																			 */
/*								PROJETO 1 – 8BITS							 */
/*					                VENDELET	                			 */
/*																			 */
/*		Profesor: Ronaldo Husemann											 */
/*																			 */
/*		Alunos:  														  	 */
/*			00304292 Arthur Antonio Soutelo Araujo							 */
/*			00301564 Gabriel Rosado dos Santos Mendes						 */
/*																			 */
/*																			 */
/*																			 */
/*							      Julho/2024								 */
/*****************************************************************************/

#include "main_header.h"

static float total_sum = 0.0;	// Define total sum variable
char buffer[25];		// Buffer to hold the uart response
char product_name[NAME_SIZE];
char product_price[NAME_SIZE];
char card_number[CARD_NUMBER_LENGTH]; // Buffer to hold the card number
char card_balance[3];
char key;
float balance;

char flag_operator_active;
char flag_porta_aberta;
volatile unsigned int buffer_index = 0;

/*****************************************************************************/
/*					           INTERRUPT                       				 */
/*****************************************************************************/

// UART RECEIVE INTERRUPT
ISR(USART0_RX_vect) {
	// Clean the buffer if it's the first byte
	if (buffer_index == 0) {
		memset(buffer, 0, sizeof(buffer));
	}

	char receivedChar = UDR0; // Read the received character
	buffer[buffer_index] = receivedChar;
	
	buffer_index++;

	// Process the buffer when enough data is received
	if (buffer_index > 3) {
		//uart_send(buffer[buffer_index-1]);
		if(buffer[0] == 'A'  && buffer[1] == 'P'){	
			unsigned int size = (unsigned char) buffer[2]; // Convert to short

			if (buffer_index == size + 3) {
				buffer[buffer_index] = '\0'; // Null-terminate the string
				buffer_index = 0; // Reset the buffer index
				analyze_serial_command(buffer, product_name, product_price, total_sum, card_number);
			}
		}
	}else if (buffer_index >= 2 && buffer[0] == 'A'  && buffer[1] != 'P') {
		switch (buffer[1]){
			case 'I':{
				if (buffer_index == 2) {
					buffer[buffer_index] = '\0'; // Null-terminate the string
					buffer_index = 0; // Reset the buffer index
					analyze_serial_command(buffer, product_name, product_price, total_sum, card_number);
				}
			}break;
			case 'R':{
				if (buffer_index == 2) {
					buffer[buffer_index] = '\0'; // Null-terminate the string
					buffer_index = 0; // Reset the buffer index
					analyze_serial_command(buffer, product_name, product_price, total_sum, card_number);
				}
			}break;
			default:{
				if (buffer_index == 3) {
					buffer[buffer_index] = '\0'; // Null-terminate the string
					buffer_index = 0; // Reset the buffer index
					analyze_serial_command(buffer, product_name, product_price, total_sum, card_number);
				}
			}break;
		}
	}
	
	// Reset the buffer index if it exceeds its size
	if (buffer_index >= sizeof(buffer)) {
		buffer_index = 0;
	}
}

// OPEN DOOR INTERRUPT
ISR(INT5_vect) {
	if(!read_door_state() && !flag_operator_active) { // DOOR IS OPEN
		_delay_ms(300);
		if(!read_door_state()){  // DOOR STILL OPEN
			reset_timer_2();
			timer2_init();	// Init timeout for operator login
			
			flag_porta_aberta = 1;
			clear_display();
			write_string_line(1, "  PORTA ABERTA");
			write_string_line(2, " REALIZAR LOGIN");
		}
	}
	//else{
		//stop_alarm();
		//display_main_menu();
		//flag_porta_aberta = 0;
	//}
}

// BLINK OPEN DOOR LED INTERRUPT
ISR(TIMER4_COMPA_vect) {
	if (flag_porta_aberta) {
		blink_led();
	}
}

// OPERATOR AND BUZZER LOGIN TIMEOUT INTERRUPT
ISR(TIMER2_COMPA_vect) {
	elapsed_time_2++;
	if (is_timeout_2() && !read_door_state()){
		// Timeout occurred
		clear_display();
		write_string_line(1, "----- ERRO -----");
		write_string_line(2, "  PORTA ABERTA");
		sound_alarm();
		flag_porta_aberta = 1;
		elapsed_time_2=0;
		stop_timer2();
	}
}

// START BUZZER OPEN DOOR INTERRUPT
ISR(TIMER3_COMPA_vect) {
	// Toggle the buzzer pin
	PORTE ^= (1 << BUZZER_PIN);
}

/*****************************************************************************/
/*					     LOGICAL & MENUS FUNCTIONS  						 */
/*****************************************************************************/

// INITIALIZE COMPONENTS & TIMERS
void init_components(void){
	clear_eeprom_vectors();
	
	init_LCD();			// Inicializa o LCD
	keypad_init();		// Inicializa o Teclado
	uart_init(19200);	// Initialize the UART with desired baud rate
	buttons_init();		// Initialize coins reading
	door_init();		// Initialize door sensor reading
	
	init_door_buzzer();
	init_timer4();		// DOOR LED
	
	init_base_cards();
	init_operator();
	UCSR0B |= (1 << RXCIE0); // Ativa a interrupção de recepção
	sei(); // Habilita as interrupções globais
}

// DISPLAY MENU
void display_main_menu(void){
	clear_display();
	write_string_line(1,"    VenDELET");
	write_string_line(2,"Digite o Produto");
}

// DISPLAY COINS INSERTED
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
		
		if(*total_sum >= atof(product_price)){
			return 1;
		}
	}
	
	return 0;
	
}

// SHOW SELECTED PRODUCT AND SEND SERIAL PRODUCT DATA
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

// CARD NUMBER INPUT
void get_card_number(char *card_number){
	clear_display();
	write_string_line(1,"Digite o Cartao:");
	read_card_number(card_number);
}

// RECHARGE CARD INPUT - OPERATOR
void get_card_balance(char *card_number, char *card_balance){
	//clear_display();
	write_string_line(1,"Digite o Valor:");
	balance = read_card_balance(card_balance);
}

// CARD PAYMENT MENU
int card_payment_menu(char *card_number, char *product_price){
	short card_index;
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

// WITHDRAW MENU - OPERATOR
int withdraw_menu(char key){
	clear_display();
	write_string_line(1,"Coleta do cofre?");
	write_string_line(2,"[*]Nao    [#]Sim");
	while(1){
		key = keypad_getkey();
		if(key=='*'){
			return 0;	// Coleta não realizada
		}
		else if (key=='#'){
			clear_display();
			write_string_line(1,"COLETA FEITA");
			return 1;	// Coleta realizada
		}
	}
}

// ADD PRODUCT MENU - OPERATOR
int add_product_menu(void){
	clear_display();
	write_string_line(1,"Add produto?");
	write_string_line(2,"[*]Nao    [#]Sim");
	
	while(1){
		key = keypad_getkey();
		if(key=='*'){
		return 0;	// Coleta não realizada
		}
		else if (key=='#'){
			char product[3]; // Ajuste o tamanho conforme necessário
			read_product(product);
			
			// Define the source string
			char product_line[16] = "Num. produto: ";
			// Concatenate the source string to the destination string
			strncat(product_line, product, 3);
			
			char quantity[3]; // Ajuste o tamanho conforme necessário
			read_quantity(product_line, quantity);
			
			send_confirm_restock(product, quantity);
			//char response;
			//response = validate_user(login, pwd);
			//return response;
		}
	}
}

// OPERATOR LOGIN
int operator_login(void){
	clear_display();
	write_string_line(1, " MODO OPERADOR");
	_delay_ms(3000);
	
	char login[7]; // Ajuste o tamanho conforme necessário
	read_login(login);
	
	// Define the source string
	char login_line[16] = "User: ";
	// Concatenate the source string to the destination string
	strncat(login_line, login, LOGIN_SIZE);
	
	char pwd[7]; // Ajuste o tamanho conforme necessário
	read_pwd(login_line, pwd);
	
	char response;
	response = validate_user(login, pwd);
	return response;
}

// OPERATOR MENU INPUT
void get_menu_operator(void) {
	const char *options[] = {
		"Adicionar Cartao",
		//"Remover Cartao",
		"Carregar Cartao",
		"Abastecer Maq.",
		"Retirar Caixa",
		"Sair"
	};
	char num_options = sizeof(options) / sizeof(options[0]);
	char current_option = navigate_options(options, num_options);
	
	// Chama a função correspondente à opção selecionada
	switch (current_option) {
		// Adicionar Cartao
		case 0: {
			get_card_number(card_number);
			add_new_card(card_number, 0.00);
			send_add_new_card(card_number);
		} break;
		// Remover Cartao
		//case 1: {
			//get_card_number(card_number);
			//remove_card(card_number);
		//} break;
		// Carregar Cartao
		case 1: {
			get_card_number(card_number);
			clear_display();
			get_card_balance(card_number, card_balance);
			update_card_balance(card_number, balance);
			send_update_card_balance(card_number, card_balance);
		} break;
		// Abastecer Maquina
		case 2: {
			add_product_menu();
		} break;
		// Retirar Caixa
		case 3: {
			withdraw_menu(key);
			send_confirm_cash_withdraw();
		} break;
		// Sair
		case 4: {
			flag_operator_active = 0;
			if(!read_door_state()){ // Active low (1 = closed, 0 = open)
				 flag_porta_aberta = 1;
				 reset_timer_2();
				 timer2_init();	// Init timeout for operator login
			}else{
				stop_alarm();
			}
			display_main_menu();
			_delay_ms(100);
		} break;
		default: {
			display_main_menu();
		} break;
	}
}

// SERIAL DATA PROCESSING CASES
void analyze_serial_command(unsigned char *buffer, char *product_name, char *product_price, float total_sum, char *card_number) {
	switch (buffer[0]) {
		case 'A':
		switch (buffer[1]) {
			case 'P': { // Product Information
				get_name_from_buffer(buffer, product_name);
				
				// Verifica se a string é igual
				if (strcmp(product_name, "Nao localizado.") != 0) {
					
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
						if(key == '#'){
							break;
						}
					}
					// Compra por Dinheiro
					if (key == '1') {
						char result = get_coins_menu(&total_sum, product_price);
						if (result == 1) {
							send_confirm_cash_purchase();
							} else {
							clear_display();
							write_string_line(1, "Compra Cancelada");
							write_string_line(2, " Tempo Excedido");
							_delay_ms(3000);
							display_main_menu();
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
							_delay_ms(3000);
							display_main_menu();
						}
					}
					} else {
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, " NAO LOCALIZADO");
					_delay_ms(3000);
					display_main_menu();
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
					display_main_menu();
					break;
					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "PRODUTO INVALIDO");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '2' - Compra com falha (quantidade insuficiente)
					case '2':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "QTD INSUFICIENTE");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '3' - Compra com falha (validade vencida)
					case '3':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "VALIDADE VENCIDA");
					_delay_ms(3000);
					display_main_menu();
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
						display_main_menu();
					} break;
					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "PRODUTO INVALIDO");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '2' - Compra com falha (quantidade insuficiente)
					case '2':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "QTD INSUFICIENTE");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '3' - Compra com falha (validade vencida)
					case '3':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "VALIDADE VENCIDA");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '4' - Compra com falha (cartão inválido)
					case '4':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "CARTAO INVALIDO");
					_delay_ms(3000);
					display_main_menu();
					break;
				}
			} break;

			case 'Q': { // Quantity - Result
				switch (buffer[2]) {
					// '0' - Compra efetivada com sucesso
					case '0':
					clear_display();
					write_string_line(1, "  Atualizacao");
					write_string_line(2, "   Realizada");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '1' - Compra com falha (produto inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "PRODUTO INVALIDO");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '2' - Compra com falha (quantidade inválida)
					case '2':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "  QTD INVALIDA");
					_delay_ms(3000);
					display_main_menu();
					break;
				}
			} break;

			case 'R': { // Cash Removal - Result
				clear_display();
				write_string_line(1, " Cofre Coletado");
				write_string_line(2, " Ate a proxima!");
				_delay_ms(3000);
				display_main_menu();
			} break;

			case 'A': { // Update Card - Result
				switch (buffer[2]) {
					// '0' - Atualização efetivada com sucesso
					case '0':
					clear_display();
					write_string_line(1, "     Saldo");
					write_string_line(2, "   Atualizado");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '1' - Atualização com falha (valor inválido)
					case '1':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, " VALOR INVALIDO");
					_delay_ms(3000);
					display_main_menu();
					break;
					// '4' - Atualização com falha (cartão inválido)
					case '4':
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, "CARTAO INVALIDO");
					_delay_ms(3000);
					display_main_menu();
					break;
				}
			} break;

			case 'I': { // Add Card - Result
				clear_display();
				write_string_line(1, "     Cartao");
				write_string_line(2, "   Adicionado");
				_delay_ms(3000);
				display_main_menu();
			} break;
		}
		break;
	}
}

/***************************************************************************/
/***************************** MAIN PROGRAM CODE ***************************/
/***************************************************************************/

int main(void){
	flag_porta_aberta = 0;
	init_components();
	
	// Clean the buffer
	if (buffer_index == 0) {
		memset(buffer, 0, sizeof(buffer));
	}
		
	stop_alarm();
	display_main_menu();


	while(1){
		// DOOR IS CLOSED BUT FLAG IS OPEN (DOOR JUST CLOSED)
		if(read_door_state() && flag_porta_aberta) {
			display_main_menu();
			flag_porta_aberta = 0;
			PORTH &= ~(1 << PH3); // Stop ED
			stop_alarm();
		}
		// DOOR IS CLOSED FLAG IS OPEN AND OPERATOR IS NOT LOGGED IN
		if(read_door_state() && flag_porta_aberta && !flag_operator_active){ 
			sound_alarm();
			clear_display();
			write_string_line(1, "----- ERRO -----");
			write_string_line(2, "PORTA ABERTA");
			_delay_ms(3000);
		}
		
		// Menu Operador
		if(flag_operator_active) {
			get_menu_operator();
		}
		total_sum = 0.0;
		key = keypad_getkey();
		// Seleção do produto pelo codigo
		if(key!=0){
			// MODO OPERADOR
			if(key=='D'){	
				char response;
				response = operator_login();
				if (response){
					flag_operator_active = 1;
					stop_alarm();
					flag_porta_aberta = 0;
					get_menu_operator();	
				}
				else{
					clear_display();
					write_string_line(1, "----- ERRO -----");
					write_string_line(2, " LOGIN INVALIDO");
					_delay_ms(3000);
					display_main_menu();
				}
			}
			// SELEÇÃO PRODUTO
			else{
				get_selected_product_menu(key);	
			}
		}
			

}

	return 0;
}