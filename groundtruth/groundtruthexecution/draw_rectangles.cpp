//Declaracao das bibliotecas
#include "cv.h"  
#include "highgui.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

void help() {
	cout << "This is the program designed to help in drawing the rectangles on the cars\n"
			"Usage:\n"
			"\t./getGT </path/to/video/file>\n\n"

			"The program shall stop on each frame, waiting for the --> spacebar\n"

			"When you`re done whith the frame, press the spacebar to call another one\n"
			"If you`re done whith the work, press 'esc' to quit the program\n\n"
	<< endl;
}

//----Global Variables
//--For selecting a region onMouseCallback
Mat frame;
int counterFrame = 0;
FILE * fp;


int main(int argc, char* argv[]) {
	help();

	int j;
	for (j = 0; argv[1][j] != '\0'; j++) {
	}
	bool running = true;
	char arquivoTxt[10];
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

	fp = fopen (arquivoTxt,"r");
	
	//Descarte do primeiro frame
	VideoCapture capture;
	capture.open( argv[1] );
	capture.read(frame);


	namedWindow("Video Capture");

	//Variaveis de contagem para auxiliar no desenho dos retangulos
	counterFrame = 2;
	counterFramePlus = 3;


	while(running && (!feof(fp))) {
		if (capture.read(frame)) {
			//Estrutura condicional que verifica se o frame atual eh diferente do segundo, pois se for realiza o desenho do ultimo
			//retangulo adquirido no procedimento de fscanf
			if (counterFrame != 2) {
				rectangle(frame,Rect(x,y,width,height),Scalar(0,0,255),1,8,0);
			}
			//Estrutura de laço de repeticao para varrer o arquivo .txt capturando as informacoes de posicionamento
			//para realizar o desenho dos retangulos nos frames
			while (counterFrame != counterFramePlus && (!feof(fp))) {
				fscanf(fp,"%d;%d;%d;%d;%d;%d;%d",&counterFrame, &x, &y, &width, &height, &centerX, &centerY);
				if (counterFrame != counterFramePlus) {
					rectangle(frame,Rect(x,y,width,height),Scalar(0,0,255),1,8,0);
				}
			}

			//Incremento da variavel que indica o proximo valor de frame (auxiliar de contagem)
			counterFramePlus = counterFramePlus + 1;
			
			//Exibicao do frame desenhado
			imshow("Video Capture", frame);
			char c;
			do {
				c = waitKey(0);

				if(c == 27) {// 'esc'
					running = false;
				}

			} while(c != 32 && running);
		} else {
			running = false;
		}
	}

	fclose(fp);
	return 0;
}
