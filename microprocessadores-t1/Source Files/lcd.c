#include "lcd.h"

// Set the data pins to output
void init_LCD(void) {
	LCD_DDR |= (1 << RS_PIN) | (1 << E_PIN) | (1 << D4_PIN) | (1 << D5_PIN) | (1 << D6_PIN) | (1 << D7_PIN);
	delay_ms(15);				// Wait for LCD to power up
	write_command_LCD(0x02);	// Initialize in 4-bit mode
	write_command_LCD(0x28);	// Function set: 4-bit/2-line
	write_command_LCD(0x0C);	// Display on, cursor off
	write_command_LCD(0x06);	// Entry mode set
	clear_display();
}

void write_data_LCD(char data) {
	// Set RS to 1 for data mode
	LCD_PORT |= (1 << RS_PIN);

	// Write high nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (data & 0xF0);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	// Write low nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (data << 4);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	delay_ms(1); // Adjust timing as needed
}

void write_command_LCD(char command) {
	// Set RS to 0 for command mode
	LCD_PORT &= ~(1 << RS_PIN);

	// Write high nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (command & 0xF0);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	// Write low nibble
	LCD_PORT &= 0x0F;
	LCD_PORT |= (command << 4);
	LCD_PORT |= (1 << E_PIN);
	LCD_PORT &= ~(1 << E_PIN);

	delay_ms(1); // Adjust timing as needed
}

void clear_display(void) {
	write_command_LCD(0x01); // Clear display
	delay_ms(2); // Wait for command to complete
}

void write_string(char line, char text[]) {
	if (line == 1)
	write_command_LCD(0x80); // Set cursor to start of line 1
	else if (line == 2)
	write_command_LCD(0xC0); // Set cursor to start of line 2

	unsigned char c;
	for (c = 0; text[c] != '\0'; c++) {
		write_data_LCD(text[c]);
	}
}

void delay_ms(unsigned int ms) {
	_delay_ms(ms);
}
