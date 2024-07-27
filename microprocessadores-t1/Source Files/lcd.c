/*****************************************************************************/
/*						       LCD FUNCTIONS		         				 */
/*****************************************************************************/

#include "lcd.h"
#include "keypad.h"

// Função para inicializar o LCD
void init_LCD(void){
	// Configura os pinos como saídas
	DDRB |= (1 << RS_PIN) | (1 << E_PIN) | (1 << D4_PIN) | (1 << D5_PIN) | (1 << D6_PIN) | (1 << D7_PIN);

	// Inicializa o LCD
	_delay_ms(50); // Aguarda o LCD estabilizar
	write_command_LCD(0x33); // Inicialização em 4 bits
	write_command_LCD(0x32); // Configuração em 4 bits
	write_command_LCD(0x28); // Configura o LCD para 2 linhas e 5x8 caracteres
	write_command_LCD(0x0C); // Liga o display e desliga o cursor
	write_command_LCD(0x06); // Incrementa o cursor
	write_command_LCD(0x01); // Limpa o display
	_delay_ms(2);            // Aguarda o display limpar
}


// Função para enviar um comando ao LCD
void write_command_LCD(uint8_t command) {
	// Envia o comando de 4 bits mais significativos
	PORTB = (PORTB & 0x0F) | (command & 0xF0);
	PORTB &= ~(1 << RS_PIN); // RS = 0 para comando
	PORTB |= (1 << E_PIN);   // Habilita o LCD
	_delay_us(1);
	PORTB &= ~(1 << E_PIN);  // Desabilita o LCD
	_delay_us(200);

	// Envia o comando de 4 bits menos significativos
	PORTB = (PORTB & 0x0F) | (command << 4);
	PORTB |= (1 << E_PIN);   // Habilita o LCD
	_delay_us(1);
	PORTB &= ~(1 << E_PIN);  // Desabilita o LCD
	_delay_ms(2);            // Aguarda o comando ser processado
}

// Função para enviar dados ao LCD
void write_data_LCD(uint8_t data) {
	// Envia os dados de 4 bits mais significativos
	PORTB = (PORTB & 0x0F) | (data & 0xF0);
	PORTB |= (1 << RS_PIN);  // RS = 1 para dados
	PORTB |= (1 << E_PIN);   // Habilita o LCD
	_delay_us(1);
	PORTB &= ~(1 << E_PIN);  // Desabilita o LCD
	_delay_us(200);

	// Envia os dados de 4 bits menos significativos
	PORTB = (PORTB & 0x0F) | (data << 4);
	PORTB |= (1 << RS_PIN);  // RS = 1 para dados
	PORTB |= (1 << E_PIN);   // Habilita o LCD
	_delay_us(1);
	PORTB &= ~(1 << E_PIN);  // Desabilita o LCD
	_delay_ms(2);            // Aguarda os dados serem processados
}

// Função para escrever uma string no LCD
void write_string_LCD(const char *text) {
	while (*text) {
		write_data_LCD(*text++); // Escreve cada caractere na tela
	}
}

void write_string_line(char line, const char *text) {
	if (line == 1)
		write_command_LCD(0x80); // Set cursor to start of line 1
	else if (line == 2)
		write_command_LCD(0xC0); // Set cursor to start of line 2

	write_string_LCD(text);
}

void clear_display(void) {
	write_command_LCD(0x01); // Clear display
	_delay_ms(2); // Wait for command to complete
}

// Função para exibir as opções e navegar entre elas
char navigate_options(const char *options[], char num_options) {
	char current_option = 0;

	// Exibe a opção atual na primeira linha
	clear_display();
	write_string_line(1, options[current_option]);
	// Exibe as instruções de navegação na segunda linha
	write_string_line(2, "<-B  A->   [#OK]");
	char key ;
	while (1) {
		// Espera a entrada do usuário
		key = keypad_getkey();
		if (key != 0) {
			switch (key) {
				case 'B':
				// Vai para a opção anterior
				current_option = (current_option - 1 + num_options) % num_options;
				// Exibe a opção atual na primeira linha
				clear_display();
				write_string_line(1, options[current_option]);
				// Exibe as instruções de navegação na segunda linha
				write_string_line(2, "<-B  A->   [#OK]");
				break;
				case 'A':
				// Vai para a próxima opção
				current_option = (current_option + 1) % num_options;
				// Exibe a opção atual na primeira linha
				clear_display();
				write_string_line(1, options[current_option]);
				// Exibe as instruções de navegação na segunda linha
				write_string_line(2, "<-B  A->   [#OK]");
				break;
				case '#':
					// Seleciona a opção atual
					return current_option;
				default:
					// Ignora outras teclas
				break;
			}
		}
	}
}