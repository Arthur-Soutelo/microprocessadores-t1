# Vending Machine Project (VenDELET)

## Overview

This project involves creating a vending machine system using the ATmega2560 microcontroller. The vending machine supports both cash and card payments and includes features like product selection, balance checking, and administrative operations. 

## Authors

- **Arthur Antonio Soutelo Araujo** - 00304292
- **Gabriel Rosado dos Santos Mendes** - 00301564

## Hardware

- **Microcontroller**: ATmega2560
- **LCD Display**: For user interface and status messages
- **Keypad**: For user input
- **Card Reader**: For card transactions
- **Buzzer and LEDs**: For auditory and visual feedback
- **Door Sensor**: To detect if the door is open or closed

## Features

- **Product Selection**: Choose a product by entering its code on the keypad.
- **Cash Payment**: Insert coins to pay for the selected product.
- **Card Payment**: Use a card for transactions. Supports adding and updating card balances.
- **Operator Mode**: Allows for administrative functions such as adding cards, restocking products, and cash withdrawal.
- **Error Handling**: Provides feedback for invalid operations, such as invalid product codes or insufficient funds.

## Components

### Main Code

- **UART Communication**: Handles data reception and processing.
- **LCD Display Management**: Updates the display with relevant information.
- **Keypad Input Handling**: Processes user input for product selection and menu navigation.
- **Card Management**: Handles card transactions and balance updates.
- **Error Reporting**: Displays error messages for various issues.

### Main Interrupt Service Routines (ISRs)

- **UART RX Interrupt**: Processes incoming UART data.
- **Door Sensor Interrupt**: Manages door state and login timeouts.
- **Timer Interrupts**: Controls LED blinking and buzzer alerts.

## Setup

1. **Hardware Connections**:
   - Connect the LCD, keypad, card reader, buzzer, and door sensor to the ATmega2560 as per the project's circuit diagram.

2. **Software Setup**:
   - Install Microchip Studio or any compatible IDE for ATmega2560 development.
   - Load the project files into the IDE.

3. **Compiling and Flashing**:
   - Compile the code using the IDE.
   - Flash the compiled code to the ATmega2560 microcontroller.

## Usage

1. **Starting the Machine**:
   - Power on the vending machine.
   - The main menu will be displayed on the LCD.

2. **Selecting a Product**:
   - Enter the product code on the keypad.
   - Choose payment method (cash or card).

3. **Making a Payment**:
   - **Cash**: Insert coins until the total sum matches or exceeds the product price.
   - **Card**: Enter the card number and confirm the purchase.

4. **Operator Mode**:
   - Access operator mode by entering the operator login credentials.
   - Perform administrative tasks like adding cards, restocking products, and withdrawing cash.

## Troubleshooting

- **Display Issues**: Check connections and ensure the LCD is properly initialized.
- **Keypad Input**: Verify keypad wiring and ensure correct handling of key presses.
