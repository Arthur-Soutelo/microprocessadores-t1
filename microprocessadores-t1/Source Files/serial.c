#include "serial.h"
#include "keypad.h"

/*****************************************************************************/
/*					           UART BASE FUNCTIONS  						 */
/*****************************************************************************/

// Função para inicializar a UART com a taxa de transmissão especificada
void uart_init(unsigned long baudrate) {
	// Habilita o receptor e transmissor
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Define o formato : 8 bits de dados, 1 bit de parada
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	unsigned int ubrr = F_CPU/16/baudrate - 1;
	// Define a taxa de transmissão (baud rate)
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
}

// Função para receber dados pela UART 
unsigned char uart_receive(void) {
	while (!(UCSR0A	& (1 << RXC0))); // Aguarda até que haja dados para receber
	return UDR0; // Obtém e retorna os dados recebidos do buffer
}

// Função para enviar dados pela UART
void uart_send(const unsigned char data) {
	// Aguarda pelo buffer de transmissão vazio
	while (!(UCSR0A & (1 << UDRE0)));
	// Coloca os dados no buffer, envia os dados
	UDR0 = data;
}

// Função para enviar uma string pela UART
void uart_send_string(const char *data) {
	while (*data) {
		uart_send(*data++);
	}
}

/*****************************************************************************/
/*					           SEND FUNCTIONS  	        					 */
/*****************************************************************************/

// Função para enviar a seleção de produto pela UART
void send_product_selection(ProductNumber product){
	uart_send('V');
	uart_send('P');
	uart_send(product.first_key);
	uart_send(product.second_key);
}

// Função para enviar a confirmação de compra em dinheiro pela UART
void send_confirm_cash_purchase(void){
	uart_send('V');
	uart_send('E');
}

// Função para enviar a confirmação de compra com cartão pela UART
void send_confirm_card_purchase(const char *num){
	uart_send('V');
	uart_send('C');
	for (int i = 0; i < 6; i++) {
		uart_send(num[i]);
	}
}

// Função para enviar a adição de um novo cartão pela UART
void send_add_new_card(const char *card_number) {
	uart_send('V');
	uart_send('I');
	uart_send(card_number[0]);
	uart_send(card_number[1]);
	uart_send(card_number[2]);
	uart_send(card_number[3]);
	uart_send(card_number[4]);
	uart_send(card_number[5]);
}

// Função para enviar a atualização do saldo do cartão pela UART
void send_update_card_balance(const char *card_number, const char *balance) {
	uart_send('V');
	uart_send('A');
	uart_send(card_number[0]);
	uart_send(card_number[1]);
	uart_send(card_number[2]);
	uart_send(card_number[3]);
	uart_send(card_number[4]);
	uart_send(card_number[5]);
	uart_send(balance[0]);
	uart_send(balance[1]);
}

// Função para enviar a confirmação de reabastecimento de produto pela UART
void send_confirm_restock(const char *product, const char *quantity){
	uart_send('V');
	uart_send('Q');
	uart_send(product[0]);	
	uart_send(product[1]);
	uart_send(quantity[0]);
	uart_send(quantity[1]);
}

// Função para enviar a confirmação de retirada de dinheiro pela UART
void send_confirm_cash_withdraw(void) {
	uart_send('V');
	uart_send('R');
}

/*****************************************************************************/
/*					           BUFFER FUNCTIONS  	        	    		 */
/*****************************************************************************/

// Função para obter o nome do buffer
void get_name_from_buffer(unsigned char *buffer, char *name) {
	int data_size = buffer[2]; // Tamanho dos dados
	int name_start_index = 3;  // Assumindo que o nome começa no 4º byte

	for (int i = 0; i < data_size - 3 ; i++) {
		name[i] = buffer[name_start_index + i];
		if (buffer[name_start_index + i] == '\0') {
			break; /// Para de copiar se encontrar o terminador nulo
		}
	}
}

// Função para obter o preço do buffer
void get_price_from_buffer(unsigned char *buffer, char *price) {
	short data_size = buffer[2];
	short name_start_index = 3;
	short name_end_index = name_start_index;
	short i;

	// Encontra o fim do nome (terminador nulo)
	for (i = 0; i < data_size - 3 && buffer[name_end_index] != '\0'; i++) {
		name_end_index++;
	}

	// Pula o terminador nulo
	short price_start_index = name_end_index + 1;

	// Copia o preço para o buffer de preço
	for (i = 0; i < data_size - (price_start_index-3) && i < PRICE_SIZE - 1; i++) {
		price[i] = buffer[price_start_index + i];
		if (buffer[price_start_index + i] == '\0') {
			break;
		}
	}
	price[i] = '\0'; // Adiciona terminador nulo
}

// Função para receber dados da UART
void receive_data_from_uart(unsigned char *buffer) {
	// Limpa o buffer
	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0xFF;
	}
	
	buffer[0] = uart_receive();
	buffer[1] = uart_receive();
	buffer[2] = uart_receive();
	
	if(buffer[0]=='A' && buffer[0]=='P'){
		unsigned char i = 3;
		char received_char;
		while (i < buffer[2]) { // Deixa espaço para o terminador nulo
			received_char = uart_receive(); // Função para receber um caractere
			if (received_char != 0xFF) {
				buffer[i++] = received_char;
			}
		}
		buffer[i] = '\0'; // Adiciona terminador nulo à string
	}
	else{
		buffer[3] = uart_receive();
		buffer[4] = '\0'; // Adiciona terminador nulo à string
	}
}