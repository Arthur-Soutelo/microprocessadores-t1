#include "save_to_eeprom.h"

// Declara vetores de armazenamento na EEPROM para cartões e credenciais de usuário
Card EEMEM eeprom_cards[MAX_CARDS];
UserCredentials EEMEM eeprom_users[MAX_LOGINS];

/*****************************************************************************/
/*					        EEPROM BASE FUNCTIONS  	  					     */
/*****************************************************************************/

// Função para escrever um byte de dados em um endereço específico da EEPROM
void eeprom_write(unsigned int address, unsigned char data) {
	eeprom_update_byte((uint8_t*)address, data);
}

// Função para ler um byte de dados de um endereço específico da EEPROM
unsigned char eeprom_read(unsigned int address) {
	return eeprom_read_byte((uint8_t*)address);
}	

/*****************************************************************************/
/*					            CARD FUNCTIONS  	  					     */
/*****************************************************************************/

// Função para salvar dados de cartão (número do cartão e saldo) na EEPROM
void save_card_data(uint8_t index, const char* card_number, float balance) {
	if (index < MAX_CARDS) { // Garante que o índice está dentro dos limites
		Card card;
		strncpy(card.card_number, card_number, sizeof(card.card_number) - 1);// Copia o número do cartão
		card.card_number[sizeof(card.card_number) - 1] = '\0';		// Garante a terminação nula
		card.balance = balance;
		eeprom_update_block((const void*)&card, (void*)&eeprom_cards[index], sizeof(Card));// Escreve na EEPROM
	}
}

// Função para ler dados de cartão da EEPROM
Card read_card_data(uint8_t index) {
	Card card = {0};			// Inicializa o cartão com valores zero
	if (index < MAX_CARDS) {	// Garante que o índice está dentro dos limites
		eeprom_read_block((void*)&card, (const void*)&eeprom_cards[index], sizeof(Card));	
	}
	return card;	// Retorna os dados do cartão
}

// Função para encontrar o índice de um cartão pelo seu número
short find_card_index(const char* card_number) {
	for (short i = 0; i < MAX_CARDS; i++) {
		Card card = read_card_data(i);
		if (strcmp(card.card_number, card_number) == 0) {
			return i; // Retorna o índice se o número do cartão corresponder
		}
	}
	return -1; // Retorna -1 se o número do cartão não for encontrado
}

// Função para inicializar dados base de cartões
void init_base_cards(void){
	 // Salva dados de cartão pré-definidos na EEPROM
	 save_card_data(0, "300123", 30.00);
	 save_card_data(1, "300121", 30.00);
	 save_card_data(2, "250025", 25.00);
}

// Função para encontrar um slot vazio nos dados do cartão
int find_empty_slot(void) {
	for (uint8_t i = 0; i < MAX_CARDS; i++) {
		Card card = read_card_data(i);
		if (card.card_number[0] == '\0') { // Verifica se o número do cartão está vazio (indicando um slot não utilizado)
			return i;
		}
	}
	return -1; // Retorna -1 se nenhum slot vazio for encontrado
}


// Função para adicionar um novo cartão na EEPROM
int add_new_card(const char* card_number, float balance) {
	// Verifica se o cartão já existe
	int existing_index = find_card_index(card_number);
	if (existing_index != -1) {
		return -2; // Retorna -2 se o cartão já existir
	}

	// Encontra um slot vazio na EEPROM
	int empty_slot = find_empty_slot();
	if (empty_slot == -1) {
		return -1; // Retorna -1 se nenhum slot vazio estiver disponível
	}

	// Salva os novos dados do cartão
	save_card_data(empty_slot, card_number, balance);
	return empty_slot; // Retorna o índice onde o cartão foi adicionado
}

// Função para remover um cartão da EEPROM
int remove_card(const char* card_number) {
	int index = find_card_index(card_number);
	if (index == -1) {
		return -1; // Retorna -1 se o número do cartão não for encontrado
	}

	// Limpa os dados do cartão no índice encontrado
	Card empty_card = {{0}, 0.0}; // Cria uma estrutura de dados de cartão vazia
	eeprom_update_block((const void*)&empty_card, (void*)&eeprom_cards[index], sizeof(Card));
	return index; // Retorna o índice onde o cartão foi removido
}

// Função para atualizar o saldo de um cartão
void update_card_balance(const char *card_number, float new_balance) {
	int index = find_card_index(card_number);
	if (index != -1) {
	// Cartão encontrado, atualiza o saldo
		Card card;
		eeprom_read_block(&card, &eeprom_cards[index], sizeof(Card));
		card.balance = card.balance + new_balance;
		eeprom_update_block(&card, &eeprom_cards[index], sizeof(Card));
	}
}

// Função para subtrair um valor do saldo de um cartão
char subtract_from_card_balance(const char *card_number, float amount_to_subtract) {
	int index = find_card_index(card_number);
	if (index != -1) {
		// Cartão encontrado, atualiza o saldo
		Card card;
		eeprom_read_block(&card, &eeprom_cards[index], sizeof(Card));
		
		if (card.balance >= amount_to_subtract) {
			card.balance -= amount_to_subtract;
			eeprom_update_block(&card, &eeprom_cards[index], sizeof(Card));
			return 1;// Subtração bem-sucedida
			} else {
			// Lida com saldo insuficiente
			return 0;
		}
	} 
}

/*****************************************************************************/
/*					      OPERATOR USER FUNCTIONS  	  					     */
/*****************************************************************************/

// Função para salvar credenciais de usuário na EEPROM
void save_user_credentials(uint8_t index, const char* login, const char* password) {
	if (index < MAX_LOGINS) { // Garante que o índice está dentro dos limites
		UserCredentials user;  
		strncpy(user.login, login, LOGIN_SIZE - 1);
		user.login[LOGIN_SIZE - 1] = '\0';    // Garante a terminação nula
		strncpy(user.password, password, PASSWORD_SIZE - 1);
		user.password[PASSWORD_SIZE - 1] = '\0';    // Garante a terminação nula
		eeprom_update_block((const void*)&user, (void*)&eeprom_users[index], sizeof(UserCredentials));
	}
}

// Função para ler credenciais de usuário da EEPROM
UserCredentials read_user_credentials(uint8_t index) {
	UserCredentials user = {0};// Inicializa o usuário com valores zero
	if (index < MAX_LOGINS) { // Garante que o índice está dentro dos limites
		eeprom_read_block((void*)&user, (const void*)&eeprom_users[index], sizeof(UserCredentials));
	}
	return user; // Retorna os dados do usuário
}

// Função para adicionar um novo usuário na EEPROM
char add_new_user(const char* login, const char* password) {
	for (char i = 0; i < MAX_LOGINS; i++) {
		UserCredentials user = read_user_credentials(i);
		if (user.login[0] == '\0') {// Slot vazio
			save_user_credentials(i, login, password);
			return i;  // Retorna o índice onde o usuário foi adicionado
		}
	}
	return -1; // Nenhum slot vazio encontrado
}

// Função para validar credenciais de usuário	
char validate_user(const char* login, const char* password) {
	if(login[0] != 0 && password[0] != 0){ // Garante que login e senha não estão vazios
		for (char i = 0; i < MAX_LOGINS; i++) {
			UserCredentials user = read_user_credentials(i);
			if (strcmp(user.login, login) == 0 && strcmp(user.password, password) == 0) {
				return 1; // Credenciais correspondem
			}
		}
	}
	
	return 0; // Credenciais não encontradas
}

// Função para inicializar um usuário operador
void init_operator(void){
	char login[7];
	char pwd[7];
	// Copia strings para arrays
	strncpy(login, "012987", 7 - 1);
	login[7 - 1] = '\0';  // Garante a terminação nula
	strncpy(pwd, "974896", 7 - 1);
	pwd[7 - 1] = '\0';  // Garante a terminação nula
	add_new_user(login, pwd);
}

/*****************************************************************************/
/*					             ERASE FUNCTION 	  					     */
/*****************************************************************************/

// Função para limpar vetores da EEPROM (cartões e usuários)
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