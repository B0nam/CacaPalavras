#include <stdio.h>
#include <random>
#include <iostream>
#include <time.h>
#include <cstring>

int controle = 0;
char palavra_chave[25];
char matriz[25][25];
void gerarMatriz(char (*ptr_matriz)[25][25]);
void palavrasMatriz();
void mostrarMatriz(char (*ptr_matriz)[25][25]);
void inserirMatriz(char (*ptr_matriz)[25][25], int eixo);
char sortearLetra();

int main() {
	srand(time(NULL));
	gerarMatriz(&matriz);
	palavrasMatriz();
	inserirMatriz(&matriz, 1); //eixo 1 = horizontal, 2 = vertical
	mostrarMatriz(&matriz);
	std::cout << "Palavra chave: " << palavra_chave << std::endl;
	return 0;
}

void gerarMatriz(char (*ptr_matriz)[25][25]) {
	for (int x=0; x<25; x++){
		for (int y=0; y<25; y++){
			(*ptr_matriz)[x][y] = sortearLetra();
		}
	}
}

char sortearLetra(){
	// Variavel de controle para a proporcao
	// 0 = inicializa a variavel
	// 1 = primeira vogal, proxima letra deve ser vogal
	// 2 = segunda vogal. proxima letra deve ser consoante
	// 3 = consoante, proxima letra deve ser vogal
	char letra = ' ';
	char letra_vogal[5] = {'A','E','I','O','U'};
	char letra_consoante[21] = {'B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z'};
	
	//3 letras | 2 vogais | 1 consoante
	if (controle == 0) {
		controle = rand()%3+1; //sorteia se a primeira letra sera vogal ou consoante
	}
	if (controle == 1) {
		letra = letra_vogal[rand()%5]; //vogal
		controle = 2;
	} else if (controle == 2) {
		letra = letra_vogal[rand()%5]; //vogal
		controle = 3;		
	} else if (controle == 3) {
		letra = letra_consoante[rand()%21]; //consoante
		controle = 1;
	}

	return letra;
}

void palavrasMatriz() {
	char palavra[25];
	int linha_sorteada = 0;
	char palavra_sorteada[25];
	int linha_atual = 0;
	int num_linhas = 0;
	FILE *lista_palavras;
	lista_palavras = fopen("wordlist.txt", "r");

	//verifica o arrquivo
	if (lista_palavras == NULL) {
		std::cout << "Erro ao abrir o arquivo. " << std::endl;
		exit(1);
	}

	//obtem o numero de lihas
	while (fgets(palavra, 25, lista_palavras) != NULL) {
		num_linhas++;
	}
	std::cout << "O numero de linhas do arquivo eh: " << num_linhas << std::endl;

	//volta o ponteiro ao inicio
	rewind(lista_palavras);

	//gera a linha da palavra sorteada
	linha_sorteada = rand()%num_linhas;

	//escolhe uma palavra da lista
	while (fgets(palavra, 25, lista_palavras) != NULL) {
		if (linha_atual == linha_sorteada) {
			strcpy(palavra_chave, palavra);
			break;
		}
		linha_atual++;
	}
	fclose(lista_palavras);	
}

void inserirMatriz(char (*ptr_matriz)[25][25], int eixo) {
	//gerar posicao aleatoria parar posicionar a palavra na matriz
	int pos_x = rand()%25; //linha
	int pos_y = rand()%25; //coluna
	int flag = 0;
	int tamanhopalavra = (strlen(palavra_chave)-2);

	//verificar se a palavra cabe na posicao sorteada
	while ((pos_x + tamanhopalavra) >= 25) {
		pos_x = rand()%25;
	}

	while ((pos_y + tamanhopalavra) >= 25) {
		pos_y = rand()%25;
	}
	

	//horizontal
	if (eixo == 1){
		for (int l=0; l<tamanhopalavra; l++){
			(*ptr_matriz)[pos_x][(pos_y)+flag] = '.';//palavra_chave[l];
			flag++;
		}
	}
	//vertical
	if (eixo == 2){
		for (int l=0; l<tamanhopalavra; l++){
			(*ptr_matriz)[(pos_x + flag)][pos_y] = '.';//palavra_chave[l];
			flag++;
		}
	}
}

void mostrarMatriz(char (*ptr_matriz)[25][25]) {
	std::cout << "Mostrando matriz..." << std::endl;
	for (int x=0; x<25; x++){
		for (int y=0;y<25;y++) {
			std::cout << " " << (*ptr_matriz)[x][y] << " ";
		}
			std::cout << std::endl;
	}
}	
