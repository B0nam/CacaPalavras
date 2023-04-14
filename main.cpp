#include <stdio.h>
#include <random>
#include <iostream>
#include <time.h>
#include <cstring>

int controle = 0;
char palavra_chave[25];
char palavra_encontrada[25]; //armazena a posicao inicial e a palavra encontrada
char palavra_posicao[25]; 
char matriz[25][25];
void gerarMatriz(char (*ptr_matriz)[25][25]);
void palavrasMatriz();
void mostrarMatriz(char (*ptr_matriz)[25][25]);
void inserirMatriz(char (*ptr_matriz)[25][25], int eixo);
void buscarPalavra(char (*ptr_matriz)[25][25], char palavraUsuario[25]);
int horizontal_inicial_x = -1;
int horizontal_inicial_y = -1;
int horizontal_final_x = -1;
int horizontal_final_y = -1;
int vertical_inicial_x = -1;
int vertical_inicial_y = -1;
int vertical_final_x = -1;
char sortearLetra();


int main() {
	srand(time(NULL));
	gerarMatriz(&matriz);
	palavrasMatriz();
	inserirMatriz(&matriz, 2); //eixo 1 = horizontal, 2 = vertical
	mostrarMatriz(&matriz);
	std::cout << "Palavra chave: " << palavra_chave << std::endl;

	char palavraDigitada[25];	
	std::cout << "Digite uma palavra: ";
	std::cin >> palavraDigitada;
	buscarPalavra(&matriz, palavraDigitada);

	std::cout << "Palavra encontrada: " << palavra_encontrada << std::endl;
	std::cout << "Posicao palavra: " << palavra_posicao << std::endl;
	std::cout << "horizontal_inicial_x: " << horizontal_inicial_x << std::endl;
	std::cout << "horizontal_inicial_y: " << horizontal_inicial_y << std::endl;
	std::cout << "horizontal_final_y: " << horizontal_final_y << std::endl;
	std::cout << "vertical_inicial_X: " << vertical_inicial_x << std::endl;
	std::cout << "vertical_inicial_y: " << vertical_inicial_y << std::endl;
	std::cout << "vertical_final_X: " << vertical_final_x << std::endl;


	mostrarMatriz(&matriz);

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
			(*ptr_matriz)[pos_x][(pos_y)+flag] = palavra_chave[l];
			flag++;
		}
	}
	//vertical
	if (eixo == 2){
		for (int l=0; l<tamanhopalavra; l++){
			(*ptr_matriz)[(pos_x + flag)][pos_y] = palavra_chave[l];
			flag++;
		}
	}
}

void mostrarMatriz(char (*ptr_matriz)[25][25]) {
	std::cout << "Mostrando matriz..." << std::endl;
	for (int x=0; x<25; x++){
		for (int y=0;y<25;y++) {
			if (x >= vertical_inicial_x && x <= vertical_final_x) {
				std::cout << " \033[1;32m" << (*ptr_matriz)[x][y] << "\033[0m ";
			} else {
				std::cout << " " << (*ptr_matriz)[x][y] << " ";
			}
		}
			std::cout << std::endl;
	}
}	

//busca horizontalmente e depois verticalmente na matriz
//comprime a linha e busca a palavra na linha.
void buscarPalavra(char (*ptr_matriz)[25][25],char palavraUsuario[25]) {
	char linha_comprimida[26] = { '\0' };
	char coluna_comprimida[26] = { '\0' };
	
	for (int t=0;t<25;t++){ //para as 25 linhas
		for (int y=0;y<25;y++){
			char letra = (*ptr_matriz)[t][y]; 
			strncat(linha_comprimida, &letra, 1);
		}
		//busca a palavra na linha.
		char *resultado = strstr(linha_comprimida, palavraUsuario);
		if (resultado != NULL) {
			//armazena a posicao inicial onde a palavra foi encontrada
			strcpy(palavra_posicao, resultado);
			strcpy(palavra_encontrada, palavraUsuario);
			horizontal_inicial_x = resultado - linha_comprimida;
			horizontal_final_x = horizontal_inicial_x + strlen(palavraUsuario);
			horizontal_inicial_y = t; //armazena a linha onde a palavra foi encontrada
		}
		//zera a linha_comprimida.
		std::memset(linha_comprimida, 0, sizeof(linha_comprimida));
	}
	
	for (int t=0;t<25;t++){ //para as 25 colunas
		for (int y=0;y<25;y++){
			char letra = (*ptr_matriz)[y][t]; 
			strncat(coluna_comprimida, &letra, 1);
		}
		//busca a palavra na coluna.
		char *resultado = strstr(coluna_comprimida, palavraUsuario);
		if (resultado != NULL) {
			//armazena a posicao inicial onde a palavra foi encontrada
			strcpy(palavra_posicao, resultado);
			strcpy(palavra_encontrada, palavraUsuario);
			vertical_inicial_x = resultado - coluna_comprimida;
			vertical_final_x = vertical_inicial_x + strlen(palavraUsuario);
			vertical_inicial_y = t; //armazena a coluna onde a palavra foi encontrada
		}
		//zera a linha_comprimida.
		std::memset(coluna_comprimida, 0, sizeof(coluna_comprimida));
	}
}
