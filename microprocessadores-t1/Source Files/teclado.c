void leteclado()
{
	BOTAO = 0;
	B_L1 = 0;
	B_L2 = 1;
	B_L3 = 1;
	B_L4 = 1;
	
	if(debounce(0) == 0){
		BOTAO = 1;
	}
	else if (debounce(1) == 0){
		BOTAO = 2;
	}
	else if (debounce(2) == 0){
		BOTAO = 3;
	}

}


unsigned short debounce (unsigned char Coluna){	//Le P0^0; P0^1; P0^2 e testa debounce
	char count = 0;
	char keylast = 0;
	char keynow = 1;
	
	while (count <7){
		atraso_lab4_2();
		keynow = P0&(1<<Coluna);
		
		if (keynow == keylast){
			count ++;
		}
		else{
			count = 0;
		}
		keylast = keynow;
	}
	return keynow;

}