
MODA:

void moda(float v[]){
	int T = 10; //tamanho do vetor
	int i, j, cont[T];
	float conta, moda;
	
	for(i=0;i<T;i++){
        for(j=i+1;j<T;j++){
        	
			if(v[i]==v[j]){
				cont[i]++;
					if(cont[i]>conta){
						conta=cont[i];
						moda=v[i];
					}
			}

        }
        cont[i]=0;
    }
    if(conta == 0){
    	printf("Nao existe moda\n");
	}
	else{
		printf("Moda: %.2f\n", moda);
	}

}
 

 

MEDIA:

void media(float v[]){
	int T = 10; //tamanho
	float soma=0;
	float media=0;
	int i;
	
    for(i=0;i<T;++i){
    	soma = soma + v[i];
	}
	
	media = soma/T;
	printf("Media: %.2f", media);
	
}
MEDIANA:

void mediana(float v[]){
	int T = 10; //tamanho do vetor
	int mediana;
	mediana = v[T/2-1];   //por algum motivo no meu só dava certo tirando 1 da conta
	printf("Mediana: %i", mediana);
	
}
 






## reference: https://www.clubedohardware.com.br/forums/topic/1291570-programa-em-c-que-calcula-moda-media-e-mediana/
