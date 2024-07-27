#include "money.h"
#include "save_to_eeprom.h"

// Função estática para debounce do botão
static unsigned char debounce(unsigned char pin);

/*****************************************************************************/
/*									 MOEDAS			        	    		 */
/*****************************************************************************/

// Função para lidar com cliques de botão e atualizar a soma total
void update_total_sum(float *total_sum) {
	if (button50c_clicked()) {
		*total_sum += 0.50;  // Adiciona 50 centavos
	}
	if (button1r_clicked()) {
		*total_sum += 1.00;  // Adiciona 1 real
	}
}

// Função para inicializar os botões
void buttons_init(void) {
	// Configura os pinos dos botões como entradas com resistores pull-down
	DDRH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
	PORTH &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN));
}

// Função para verificar se o botão de 50 centavos foi clicado
unsigned char button50c_clicked(void) {
	return debounce(BUTTON1_PIN);
}

// Função para verificar se o botão de 1 real foi clicado
unsigned char button1r_clicked(void) {
	return debounce(BUTTON2_PIN);
}

// Função estática para debounce do botão
static unsigned char debounce(unsigned char pin) {
	unsigned char count = 0;
	unsigned char keylast = 0;
	unsigned char keynow = 1;

	while (count < 7) {
		_delay_ms(5);
		keynow = (PINH & (1 << pin)) >> pin;	// Lê o estado do pino e ajusta para 0 ou 1

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

// Função para inicializar a porta
void door_init(void) {
	// Configura o pino do sensor da porta como entrada
	DDRE &= ~(1 << DOOR_PIN);
	PORTE |= (1 << DOOR_PIN); // Habilita o pull-up interno
}

// Função para ler o estado da porta
char read_door_state(void) {
	 return (PINE & (1 << DOOR_PIN));
}

// Função para inicializar o LED
void init_led(void){
	// Configura o pino do LED como saída
	DDRL |= (1 << PL7);
}

// Função para acender o LED
void turn_on_led(void){
	// Acende o LED
	PORTL |= (1 << PL7);
}

// Função para apagar o LED
void turn_off_led(void){
	// Apaga o LED
	PORTL &= ~(1 << PL7);
}