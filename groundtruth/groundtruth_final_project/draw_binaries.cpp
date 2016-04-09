//Declaracao das bibliotecas
#include "cv.h"  
#include "highgui.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//Como decidido em sala, a implementação do desenho dos quadrados na imagem é feita uma a uma, ou seja, cada retangulo de CADA frame
//é desenhado sozinho para evitar problemas na realizar do match futuro. Sendo assim temos o exemplo:
//Frame 2: caixa 1
//Frame 2: caixa 2
//Frame 3: caixa 1
//Frame 4: caixa 1
//Frame 4: caixa 2

void help() {
	cout << "This is the program designed to help in drawing filled rectangles on the cars\n"
			"Usage:\n"
			"\t./DrawBinaries </path/to/video/file>\n\n"

			"The program shall stop on each frame, waiting for the --> spacebar\n"

			"When you`re done whith the frame, press the spacebar to call another one\n"
			"If you`re done whith the work, press 'esc' to quit the program\n\n"
	<< endl;
}

//----Global Variables
int counterFrame = 0;
FILE * fp;


int main(int argc, char* argv[]) {
	help();

	int j;
	//Leitura da string de entrada analisando se a mesma é um vídeo com dois digitos de entrada ou 1
	//Exemplo: 1.avi ou 10.avi
	for (j = 0; argv[1][j] != '\0'; j++) {
	}
	bool running = true;
	char arquivoTxt[10];
	//Ajuste do nome do arquivo
	strcpy(arquivoTxt,argv[1]);
	if (j == 5) {
		arquivoTxt[1] = '_';
	} else {
		arquivoTxt[2] = '_';
	}
	strcat(arquivoTxt,".txt");

	//Variaveis de posicionamento
	int x, y, height, width, centerX, centerY, counterFramePlus;

	//Informacao do arquivo txt aberto
	cout << arquivoTxt << endl;

	//Abertura de arquivo .txt
	fp = fopen (arquivoTxt,"r");

	//Estrutura de repetição que realiza a leitura de cada linha do .txt (quadrado marcado) e pinta-o sobre uma imagem preta
	//com um retangulo completamente preenchido para posterior realização de operação AND (a princípio) para verificação
	//de match com o método de deteção de carros
	while(!feof(fp) && running == true) {
		//Criação da imagem na cor preta
		Mat binaries(240, 320, CV_8UC3, Scalar(0,0,0));
		//Leitura do arquivo .txt
		fscanf(fp,"%d;%d;%d;%d;%d;%d;%d",&counterFrame, &x, &y, &width, &height, &centerX, &centerY);
		//Desenho de um retangulo na imagem indicando um dos carros no frame atual
		rectangle(binaries,Rect(x,y,width,height),Scalar(255,255,255),CV_FILLED,8);
		//Print informando o frame atual em processamento
		cout << "Frame: " << counterFrame << endl;
		//Exibicao da imagem criada com a caixa
		imshow("Video Capture", binaries);
		char c;
		//Exibicao da imagem até que o usuario aperte ESPAÇO para prosseguir para a proxima seleção, nao necessariamente
		//será o proximo frame, ou pressione ESC para encerrar
		do {
			c = waitKey(0);
			if(c == 27) {// 'esc'
				running = false;
			}
		} while(c != 32);
	}

	//Fechamento do arquivo
	fclose(fp);
	return 0;
}
