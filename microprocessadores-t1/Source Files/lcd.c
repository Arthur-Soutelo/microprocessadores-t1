#include "lcd.h"

// Set the data pins to output
void init_LCD(void) {
	LCD_DDR |= (1 << RS_PIN) | (1 << E_PIN) | (1 << D4_PIN) | (1 << D5_PIN) | (1 << D6_PIN) | (1 << D7_PIN);
	_delay_ms(15); // Wait for LCD to power up
	escreve_comando_LCD(0x02); // Initialize in 4-bit mode
	escreve_comando_LCD(0x28); // Function set: 4-bit/2-line
	escreve_comando_LCD(0x0C); // Display on, cursor off
	escreve_comando_LCD(0x06); // Entry mode set
	limpa_display();
}

void escreve_dado_LCD(char dado) {
	// Set RS to 1 for data mode
	LCD_PORT |= (1 << RS_PIN);

	// Write high nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (dado & 0xF0);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	// Write low nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (dado << 4);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	atraso();
}

void escreve_comando_LCD(char comando) {
	// Set RS to 0 for command mode
	LCD_PORT &= ~(1 << RS_PIN);

	// Write high nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (comando & 0xF0);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	// Write low nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (comando << 4);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	atraso();
}

void limpa_display(void) {
	escreve_comando_LCD(0x01); // Clear display
	_delay_ms(2); // Wait for command to complete
}

void escreve_string(char linha, char texto[]) {
	if (linha == 1)
	escreve_comando_LCD(0x80); // Set cursor to start of line 1
	else if (linha == 2)
	escreve_comando_LCD(0xC0); // Set cursor to start of line 2

	unsigned char c;
	for (c = 0; texto[c] != '\0'; c++) {
		escreve_dado_LCD(texto[c]);
	}
}

void atraso(void) {
	_delay_us(100);
}
