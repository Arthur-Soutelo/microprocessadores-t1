#include "lcd.h"

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

//#include "lcd.h"
//#include "timer.h"
//
//// Set the data pins to output
//void init_LCD(void) {
	//LCD_DDR |= (1 << RS_PIN) | (1 << E_PIN) | (1 << D4_PIN) | (1 << D5_PIN) | (1 << D6_PIN) | (1 << D7_PIN);
	//delay_ms(500);				// Wait for LCD to power up
	//
	//// Send function set command sequence to initialize the LCD
	//write_command_LCD(0x03); // Function set: 8-bit mode
	//delay_ms(5);             // Wait
	//write_command_LCD(0x03); // Function set: 8-bit mode (repeat)
	//delay_ms(1);             // Wait
	//write_command_LCD(0x03); // Function set: 8-bit mode (repeat)
	//delay_ms(1);             // Wait
	//
	//write_command_LCD(0x02);	// Initialize in 4-bit mode
	//write_command_LCD(0x28);	// Function set: 4-bit/2-line
	//write_command_LCD(0x0C);	// Display on, cursor off
	//write_command_LCD(0x06);	// Entry mode set: increment cursor, no display shift
	//clear_display();
	//write_command_LCD(0x80); // Set cursor to start of line 1
//}
//
//void write_data_LCD(uint8_t data) {
	//// Set RS to 1 for data mode
	//LCD_PORT |= (1 << RS_PIN);
//
	//// Write high nibble
	//LCD_PORT &= 0x0F;
	//LCD_PORT |= (data & 0xF0);
	//LCD_PORT |= (1 << E_PIN);
	//delay_ms(1);
	//LCD_PORT &= ~(1 << E_PIN);
//
	//// Write low nibble
	//LCD_PORT &= 0x0F;
	//LCD_PORT |= (data << 4);
	//LCD_PORT |= (1 << E_PIN);
	//delay_ms(1);
	//LCD_PORT &= ~(1 << E_PIN);
//
	//delay_ms(2); // Adjust timing as needed
//}
//
//void write_command_LCD(uint8_t command) {
	//// Set RS to 0 for command mode
	//LCD_PORT &= ~(1 << RS_PIN);
//
	//// Write high nibble
	//LCD_PORT &= 0x0F;
	//LCD_PORT |= (command & 0xF0);
	//LCD_PORT |= (1 << E_PIN);
	//delay_ms(1);
	//LCD_PORT &= ~(1 << E_PIN);
//
	//// Write low nibble
	//LCD_PORT &= 0x0F;
	//LCD_PORT |= (command << 4);
	//LCD_PORT |= (1 << E_PIN);
	//delay_ms(1);
	//LCD_PORT &= ~(1 << E_PIN);
//
	//delay_ms(2); // Adjust timing as needed
//}
//
//void clear_display(void) {
	//write_command_LCD(0x01); // Clear display
	//delay_ms(2); // Wait for command to complete
//}
//
//void write_string(char line, char *text) {
	//if (line == 1)
	//write_command_LCD(0x80); // Set cursor to start of line 1
	//else if (line == 2)
	//write_command_LCD(0xC0); // Set cursor to start of line 2
//
	//unsigned char c;
	//for (c = 0; text[c] != '\0'; c++) {
		//write_data_LCD(text[c]);
		//write_command_LCD(0x06);	// Entry mode set
	//}
//}
//
//void delay_ms(unsigned int ms) {
	////unsigned char c;
	////for (c=0; c<40*ms; c++);
	//int i;
	//for (i = 0; i < ms; i++) {
		//_delay_us(1);
	//}
	//
	////timer0_delay_us(ms*1000);
//}
