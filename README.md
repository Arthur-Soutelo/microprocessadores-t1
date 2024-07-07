# ATmega2560 Microcontroller Project

This project demonstrates the use of an ATmega2562 microcontroller to handle multiple functionalities including keypad input, LCD display control, EEPROM manipulation, and button debouncing for calculating and storing values.

## Table of Contents

1. [Introduction](#introduction)
2. [Hardware Requirements](#hardware-requirements)
3. [Software Requirements](#software-requirements)
4. [Project Structure](#project-structure)
5. [Configuration](#configuration)
6. [Usage](#usage)
7. [Functions](#functions)
8. [Notes](#notes)
9. [License](#license)

## Introduction

This project includes code for reading keypad inputs with debouncing, displaying information on an LCD, calculating values based on button presses, and storing/retrieving these values from EEPROM. The project files are structured into separate modules for keypad handling, LCD control, EEPROM operations, and other utility functions.

## Hardware Requirements

- ATmega2560 microcontroller
- 4x4 keypad
- 16x2 LCD display
- Two buttons with pull-down resistors
- Microchip Studio for AVR programming

## Software Requirements

- Microchip Studio
- AVR-GCC compiler

## Project Structure

The project consists of the following files:

- `main.c`: The main file containing the main loop and example usage.
- `keypad.h` & `keypad.c`: Header and source files for keypad handling functions.
- `lcd.h` & `lcd.c`: Header and source files for LCD control functions.
- `money.h` & `money.c`: Header and source files for calculating and handling monetary values.
- `save_to_eeprom.h` & `save_to_eeprom.c`: Header and source files for EEPROM handling functions.
- `serial.h` & `serial.c`: Header and source files for serial communication functions.
- `timer.h` & `timer.c`: Header and source files for timer utility functions.
- `main_header.h`: General header file for global definitions.
- `system_libraries.h`: Header file for including necessary system libraries.

## Configuration

### Pin Definitions

In `keypad.h`:
```c
#define KEYPAD_ROW_PORT  PORTA
#define KEYPAD_COL_PORT  PORTB
#define KEYPAD_ROW_DDR   DDRA
#define KEYPAD_COL_DDR   DDRB
#define KEYPAD_ROW_PIN   PINA
#define KEYPAD_COL_PIN   PINB
```

In `lcd.h`:
```c
#define RS_PIN     PB0
#define E_PIN      PB1
#define D4_PIN     PB4
#define D5_PIN     PB5
#define D6_PIN     PB6
#define D7_PIN     PB7
#define LCD_PORT   PORTB
#define LCD_DDR    DDRB
```

### EEPROM Address

In `money.h`:
```c
#define TOTAL_VALUE_ADDR    0x00 // EEPROM address to store the total value
```

## Usage

1. Clone or download the project files.
2. Open Microchip Studio and create a new project for the ATmega2562 microcontroller.
3. Add the provided source and header files to your project.
4. Connect the hardware components as per the pin configurations.
5. Compile and upload the project to your ATmega2562 microcontroller.

### Example Usage

In `main.c`:
```c
#include <avr/io.h>
#include <util/delay.h>
#include "buttons.h"
#include "save_to_eeprom.h"
#include "money.h"
#include "lcd.h"
#include "keypad.h"

int main(void) {
    unsigned char total = 0;

    // Initialize peripherals
    buttons_init();
    lcd_init();
    keypad_init();

    // Load previously saved total from EEPROM
    total = eeprom_read(TOTAL_VALUE_ADDR);

    while (1) {
        if (button1_clicked()) {
            total += BUTTON1_VALUE;
        }

        if (button2_clicked()) {
            total += BUTTON2_VALUE;
        }

        // Display total on LCD
        lcd_clear();
        lcd_write_string("Total: ");
        lcd_write_value(total / 100.0); // Assuming total is in cents

        // Save total to EEPROM periodically
        eeprom_write(TOTAL_VALUE_ADDR, total);

        _delay_ms(100);
    }

    return 0;
}
```

## Functions

### Button Handling Functions

- `void buttons_init(void)`: Initializes button pins as inputs with pull-down resistors.
- `unsigned char button1_clicked(void)`: Returns `1` if Button 1 is clicked, `0` otherwise.
- `unsigned char button2_clicked(void)`: Returns `1` if Button 2 is clicked, `0` otherwise.

### EEPROM Handling Functions

- `void eeprom_write(unsigned int address, unsigned char data)`: Writes a byte to EEPROM at the specified address.
- `unsigned char eeprom_read(unsigned int address)`: Reads a byte from EEPROM at the specified address.

### Keypad Handling Functions

- `void keypad_init(void)`: Initializes the keypad pins.
- `unsigned char keypad_get_key(void)`: Returns the key pressed on the keypad.

### LCD Control Functions

- `void lcd_init(void)`: Initializes the LCD.
- `void lcd_clear(void)`: Clears the LCD display.
- `void lcd_write_string(char *str)`: Writes a string to the LCD.
- `void lcd_write_value(float value)`: Writes a float value to the LCD.

### Timer Utility Functions

- `void timer_init(void)`: Initializes the timer.
- `void timer_delay_us(unsigned int us)`: Delays execution by the specified number of microseconds.

## Notes

- Ensure that the buttons are connected to the specified pins with pull-down resistors.
- Adjust the debounce delay in `debounce()` function if necessary to suit your hardware configuration.
- EEPROM write operations have a limited number of write cycles. Minimize frequent writes to extend EEPROM life.
