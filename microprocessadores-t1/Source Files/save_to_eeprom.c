#include "save_to_eeprom.h"

// Declara vetores de armazenamento na EEPROM para cart�es e credenciais de usu�rio
Card EEMEM eeprom_cards[MAX_CARDS];
UserCredentials EEMEM eeprom_users[MAX_LOGINS];

/*****************************************************************************/
/*					        EEPROM BASE FUNCTIONS  	  					     */
/*****************************************************************************/

// Fun��o para escrever um byte de dados em um endere�o espec�fico da EEPROM
void eeprom_write(unsigned int address, unsigned char data) {
	eeprom_update_byte((uint8_t*)address, data);
}

// Fun��o para ler um byte de dados de um endere�o espec�fico da EEPROM
unsigned char eeprom_read(unsigned int address) {
	return eeprom_read_byte((uint8_t*)address);
}	

/*****************************************************************************/
/*					            CARD FUNCTIONS  	  					     */
/*****************************************************************************/

// Fun��o para salvar dados de cart�o (n�mero do cart�o e saldo) na EEPROM
void save_card_data(uint8_t index, const char* card_number, float balance) {
	if (index < MAX_CARDS) { // Garante que o �ndice est� dentro dos limites
		Card card;
		strncpy(card.card_number, card_number, sizeof(card.card_number) - 1);// Copia o n�mero do cart�o
		card.card_number[sizeof(card.card_number) - 1] = '\0';		// Garante a termina��o nula
		card.balance = balance;
		eeprom_update_block((const void*)&card, (void*)&eeprom_cards[index], sizeof(Card));// Escreve na EEPROM
	}
}

// Fun��o para ler dados de cart�o da EEPROM
Card read_card_data(uint8_t index) {
	Card card = {0};			// Inicializa o cart�o com valores zero
	if (index < MAX_CARDS) {	// Garante que o �ndice est� dentro dos limites
		eeprom_read_block((void*)&card, (const void*)&eeprom_cards[index], sizeof(Card));	
	}
	return card;	// Retorna os dados do cart�o
}

// Fun��o para encontrar o �ndice de um cart�o pelo seu n�mero
short find_card_index(const char* card_number) {
	for (short i = 0; i < MAX_CARDS; i++) {
		Card card = read_card_data(i);
		if (strcmp(card.card_number, card_number) == 0) {
			return i; // Retorna o �ndice se o n�mero do cart�o corresponder
		}
	}
	return -1; // Retorna -1 se o n�mero do cart�o n�o for encontrado
}

// Fun��o para inicializar dados base de cart�es
void init_base_cards(void){
	 // Salva dados de cart�o pr�-definidos na EEPROM
	 save_card_data(0, "300123", 30.00);
	 save_card_data(1, "300121", 30.00);
	 save_card_data(2, "250025", 25.00);
}

// Fun��o para encontrar um slot vazio nos dados do cart�o
int find_empty_slot(void) {
	for (uint8_t i = 0; i < MAX_CARDS; i++) {
		Card card = read_card_data(i);
		if (card.card_number[0] == '\0') { // Verifica se o n�mero do cart�o est� vazio (indicando um slot n�o utilizado)
			return i;
		}
	}
	return -1; // Retorna -1 se nenhum slot vazio for encontrado
}


// Fun��o para adicionar um novo cart�o na EEPROM
int add_new_card(const char* card_number, float balance) {
	// Verifica se o cart�o j� existe
	int existing_index = find_card_index(card_number);
	if (existing_index != -1) {
		return -2; // Retorna -2 se o cart�o j� existir
	}

	// Encontra um slot vazio na EEPROM
	int empty_slot = find_empty_slot();
	if (empty_slot == -1) {
		return -1; // Retorna -1 se nenhum slot vazio estiver dispon�vel
	}

	// Salva os novos dados do cart�o
	save_card_data(empty_slot, card_number, balance);
	return empty_slot; // Retorna o �ndice onde o cart�o foi adicionado
}

// Fun��o para remover um cart�o da EEPROM
int remove_card(const char* card_number) {
	int index = find_card_index(card_number);
	if (index == -1) {
		return -1; // Retorna -1 se o n�mero do cart�o n�o for encontrado
	}

	// Limpa os dados do cart�o no �ndice encontrado
	Card empty_card = {{0}, 0.0}; // Cria uma estrutura de dados de cart�o vazia
	eeprom_update_block((const void*)&empty_card, (void*)&eeprom_cards[index], sizeof(Card));
	return index; // Retorna o �ndice onde o cart�o foi removido
}

// Fun��o para atualizar o saldo de um cart�o
void update_card_balance(const char *card_number, float new_balance) {
	int index = find_card_index(card_number);
	if (index != -1) {
	// Cart�o encontrado, atualiza o saldo
		Card card;
		eeprom_read_block(&card, &eeprom_cards[index], sizeof(Card));
		card.balance = card.balance + new_balance;
		eeprom_update_block(&card, &eeprom_cards[index], sizeof(Card));
	}
}

// Fun��o para subtrair um valor do saldo de um cart�o
char subtract_from_card_balance(const char *card_number, float amount_to_subtract) {
	int index = find_card_index(card_number);
	if (index != -1) {
		// Cart�o encontrado, atualiza o saldo
		Card card;
		eeprom_read_block(&card, &eeprom_cards[index], sizeof(Card));
		
		if (card.balance >= amount_to_subtract) {
			card.balance -= amount_to_subtract;
			eeprom_update_block(&card, &eeprom_cards[index], sizeof(Card));
			return 1;// Subtra��o bem-sucedida
			} else {
			// Lida com saldo insuficiente
			return 0;
		}
	} 
}

/*****************************************************************************/
/*					      OPERATOR USER FUNCTIONS  	  					     */
/*****************************************************************************/

// Fun��o para salvar credenciais de usu�rio na EEPROM
void save_user_credentials(uint8_t index, const char* login, const char* password) {
	if (index < MAX_LOGINS) { // Garante que o �ndice est� dentro dos limites
		UserCredentials user;  
		strncpy(user.login, login, LOGIN_SIZE - 1);
		user.login[LOGIN_SIZE - 1] = '\0';    // Garante a termina��o nula
		strncpy(user.password, password, PASSWORD_SIZE - 1);
		user.password[PASSWORD_SIZE - 1] = '\0';    // Garante a termina��o nula
		eeprom_update_block((const void*)&user, (void*)&eeprom_users[index], sizeof(UserCredentials));
	}
}

// Fun��o para ler credenciais de usu�rio da EEPROM
UserCredentials read_user_credentials(uint8_t index) {
	UserCredentials user = {0};// Inicializa o usu�rio com valores zero
	if (index < MAX_LOGINS) { // Garante que o �ndice est� dentro dos limites
		eeprom_read_block((void*)&user, (const void*)&eeprom_users[index], sizeof(UserCredentials));
	}
	return user; // Retorna os dados do usu�rio
}

// Fun��o para adicionar um novo usu�rio na EEPROM
char add_new_user(const char* login, const char* password) {
	for (char i = 0; i < MAX_LOGINS; i++) {
		UserCredentials user = read_user_credentials(i);
		if (user.login[0] == '\0') {// Slot vazio
			save_user_credentials(i, login, password);
			return i;  // Retorna o �ndice onde o usu�rio foi adicionado
		}
	}
	return -1; // Nenhum slot vazio encontrado
}

// Fun��o para validar credenciais de usu�rio	
char validate_user(const char* login, const char* password) {
	if(login[0] != 0 && password[0] != 0){ // Garante que login e senha n�o est�o vazios
		for (char i = 0; i < MAX_LOGINS; i++) {
			UserCredentials user = read_user_credentials(i);
			if (strcmp(user.login, login) == 0 && strcmp(user.password, password) == 0) {
				return 1; // Credenciais correspondem
			}
		}
	}
	
	return 0; // Credenciais n�o encontradas
}

// Fun��o para inicializar um usu�rio operador
void init_operator(void){
	char login[7];
	char pwd[7];
	// Copia strings para arrays
	strncpy(login, "012987", 7 - 1);
	login[7 - 1] = '\0';  // Garante a termina��o nula
	strncpy(pwd, "974896", 7 - 1);
	pwd[7 - 1] = '\0';  // Garante a termina��o nula
	add_new_user(login, pwd);
}

/*****************************************************************************/
/*					             ERASE FUNCTION 	  					     */
/*****************************************************************************/

// Fun��o para limpar vetores da EEPROM (cart�es e usu�rios)
void clear_eeprom_vectors(void) {
	// Limpa eeprom_cards
	for (uint8_t i = 0; i < MAX_CARDS; i++) {
		Card empty_card = {{0}, 0.0};
		eeprom_update_block((const void*)&empty_card, (void*)&eeprom_cards[i], sizeof(Card));
	}

	// Limpa eeprom_users
	for (uint8_t i = 0; i < MAX_LOGINS; i++) {
		UserCredentials empty_user = {{0}, {0}};
		eeprom_update_block((const void*)&empty_user, (void*)&eeprom_users[i], sizeof(UserCredentials));
	}
}