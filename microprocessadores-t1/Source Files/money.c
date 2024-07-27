#include "money.h"
#include "save_to_eeprom.h"

// Fun��o est�tica para debounce do bot�o
static unsigned char debounce(unsigned char pin);

/*****************************************************************************/
/*									 MOEDAS			        	    		 */
/*****************************************************************************/

// Fun��o para lidar com cliques de bot�o e atualizar a soma total
void update_total_sum(float *total_sum) {
	if (button50c_clicked()) {
		*total_sum += 0.50;  // Adiciona 50 centavos
	}
	if (button1r_clicked()) {
		*total_sum += 1.00;  // Adiciona 1 real
	}
}

// Fun��o para inicializar os bot�es
void buttons_init(void) {
	// Configura os pinos dos bot�es como entradas com resistores pull-down
	DDRH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
	PORTH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
}

// Fun��o para verificar se o bot�o de 50 centavos foi clicado
unsigned char button50c_clicked(void) {
	return debounce(BUTTON1_PIN);
}

// Fun��o para verificar se o bot�o de 1 real foi clicado
unsigned char button1r_clicked(void) {
	return debounce(BUTTON2_PIN);
}

// Fun��o est�tica para debounce do bot�o
static unsigned char debounce(unsigned char pin) {
	unsigned char count = 0;
	unsigned char keylast = 0;
	unsigned char keynow = 1;

	while (count < 7) {
		_delay_ms(5);
		keynow = (PINH & (1 << pin)) >> pin;	// L� o estado do pino e ajusta para 0 ou 1

		if (keynow == keylast) {
			count++;
			} else {
			count = 0;
		}

		keylast = keynow;
	}

	return keynow;
}

// Passa para float
float atof_with_comma(const char* str) {
	// Create a copy of the input string
	char* copy = strdup(str);
	if (copy == NULL) {
		// Handle memory allocation failure
		return 0.0;
	}

	// Replace comma with dot
	for (char* p = copy; *p != '\0'; p++) {
		if (*p == ',') {
			*p = '.';
		}
	}

	// Convert the modified string to float
	float result = atof(copy);

	// Free the allocated memory
	free(copy);

	return result;
}

/*****************************************************************************/
/*										  PORTA	  	        	    		 */
/*****************************************************************************/

// Fun��o para inicializar a porta
void door_init(void) {
	// Configura o pino do sensor da porta como entrada
	DDRE &= ~(1 << DOOR_PIN);
	PORTE |= (1 << DOOR_PIN); // Habilita o pull-up interno
}

// Fun��o para ler o estado da porta
char read_door_state(void) {
	 return (PINE & (1 << DOOR_PIN));
}

// Fun��o para inicializar o LED
void init_led(void){
	// Configura o pino do LED como sa�da
	DDRL |= (1 << PL7);
}

// Fun��o para acender o LED
void turn_on_led(void){
	// Acende o LED
	PORTL |= (1 << PL7);
}

// Fun��o para apagar o LED
void turn_off_led(void){
	// Apaga o LED
	PORTL &= ~(1 << PL7);
}