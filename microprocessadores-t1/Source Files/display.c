	//
//void escreve_dado_LCD(char dado){
	//RS = 1;
	//P1 &= 0x0f;
	//P1 |= (dado & 0xf0);
	//E=1;
	//E=0;
	//P1 &= 0x0f;
	//P1 |= (dado << 4);
	//E=1;
	//E=0;
	//atraso();
//}
//
//void escreve_comando_LCD(char comando){
	//RS = 0;
	//P1 &= 0x0f;
	//P1 |= (comando & 0xf0);
	//E=1;
	//E=0;
	//P1 &= 0x0f;
	//P1 |= (comando << 4);
	//E=1;
	//E=0;
	//atraso();
//}
//
//void limpa_display(){
	//unsigned short c;
	//escreve_comando_LCD(0x01);	// limpa display
	//for (c=0; c<1800; c++);
//}
//
//void escreve_string(char nome[]){
	//unsigned char c;
	//for (c=0; c < 8; c++){
		//if (nome[c] != '\0') {
			//escreve_dado_LCD(nome[c]);
			//escreve_comando_LCD(0x06); //incementa pos
			//} else {
			//break;
		//}
	//}
	//
//}