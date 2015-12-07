//Declaracao das bibliotecas
#include "cv.h"  
#include "highgui.h"
#include <strings.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


typedef struct {
	int framegt;
	int xgt;
	int ygt;
	int heightgt;
	int widthgt;
	int centerXgt;
	int centerYgt;
} dadosGT;

typedef struct {
	int framesaida;
	int xsaida;
	int ysaida;
	int heightsaida;
	int widthsaida;
	int centerXsaida;
	int centerYsaida;
} dadosSaida;

double getMatch (Mat gt, Mat saida);
double getMajor ();
void printResultados ();
void getFile ();

void help() {
	cout << "This is the program designed to help creating our Ground Truth data\n"
			"Usage:\n"
			"\t./groundtruth </path/to/video/file>\n\n"

			"The program shall stop on each frame, waiting for the --> spacebar\n"
			"In the meanwhile, select the ROI by dragging the mouse around it.\n\n"

			"When you`re done whith the frame, press the spacebar to call another one\n"
			"If you`re done whith the work, press 'esc' to quit the program\n\n"
	<< endl;
}

//----Global Variables
//--For selecting a region onMouseCallback
int counterFramePlus = 3;
FILE * fp;
FILE * fp2;
double * match;
int entradas = 0;
double matches[60][30];
int gtDetected[60];
int contadorAuxiliar = 3;

int main(int argc, char* argv[]) {
	help();


	vector<dadosGT> dadosGTX;
	vector<dadosSaida> dadosSaidaX;

	int j;
	for (j = 0; argv[1][j] != '\0'; j++) {
	}
	bool running = true;
	char arquivoTxt[10];
	char arquivoTxt2[10];


	//-----------------------------------
	strcpy(arquivoTxt,argv[1]);
	if (j == 5) {
		arquivoTxt[1] = '_';
	} else {
		arquivoTxt[2] = '_';
	}
	strcat(arquivoTxt,".txt");
	//-----------------------------------
	strcpy(arquivoTxt2,argv[1]);
	if (j == 5) {
		arquivoTxt2[1] = '_';
	} else {
		arquivoTxt2[2] = '_';
	}
	strcat(arquivoTxt2,"2.txt");
	//-----------------------------------

	cout << arquivoTxt << endl;
	cout << arquivoTxt2 << endl;

	fp = fopen (arquivoTxt,"r");
	fp2 = fopen (arquivoTxt2,"r");
	int counterFrame, x, y, width, height, centerX, centerY;

	if (fp != NULL && fp2 != NULL) {
		cout << "Arquivos abertos com sucesso!" << endl;
	} else {
		cout << "Falha na abertura dos arquivos!" << endl;
	}

	//----------------------------------------------------------------
	//--------------Passagem dos dados do TXT para memória------------
	dadosGT reg1;
	dadosSaida reg2;
	rewind(fp);
	rewind(fp2);
	fscanf(fp,"%d;%d;%d;%d;%d;%d;%d",&reg1.framegt, &reg1.xgt, &reg1.ygt, &reg1.widthgt, &reg1.heightgt, &reg1.centerXgt, &reg1.centerYgt);
	while (!feof(fp)) {
		if (!feof(fp)) {
			dadosGTX.push_back(reg1);			
			fscanf(fp,"%d;%d;%d;%d;%d;%d;%d",&reg1.framegt, &reg1.xgt, &reg1.ygt, &reg1.widthgt, &reg1.heightgt, &reg1.centerXgt, &reg1.centerYgt);
		}
	}

	fscanf(fp2,"%d;%d;%d;%d;%d;%d;%d",&reg2.framesaida, &reg2.xsaida, &reg2.ysaida, &reg2.widthsaida, &reg2.heightsaida, &reg2.centerXsaida, &reg2.centerYsaida);	
	while (!feof(fp2)) {
		if (!feof(fp2)) {
			if ((reg2.xsaida >= 123 && reg2.widthsaida <= 196) && (reg2.ysaida >= 68 && reg2.heightsaida <= 154)) {
				dadosSaidaX.push_back(reg2);
				fscanf(fp2,"%d;%d;%d;%d;%d;%d;%d",&reg2.framesaida, &reg2.xsaida, &reg2.ysaida, &reg2.widthsaida, &reg2.heightsaida, &reg2.centerXsaida, &reg2.centerYsaida);
			}		
		}
	}

	//-----------ITERADOR PARA VARRER VECTOR, TOOOPSSIMO!
	//vector<dadosGT>::iterator aux;
	//for (aux = dadosGTX.begin(); aux != dadosGTX.end(); aux++) {
	//	cout << aux->framegt << endl;
	//}


	namedWindow("GT",WINDOW_NORMAL);
	namedWindow("Saida",WINDOW_NORMAL);

	moveWindow("GT",400,200);
	moveWindow("Saida",800,200);

	
	vector<dadosGT>::iterator gts = dadosGTX.begin();
	int flag = 0;
	int frame = 0;
	int selecao = 0;
	while(gts != dadosGTX.end() && running == true) {
		vector<dadosSaida>::iterator saidas = dadosSaidaX.begin();
		//Criação da imagem na cor preta
		Mat binariesGT(240, 320, CV_8UC1, Scalar(0,0,0));
		//Desenho de um retangulo na imagem indicando um dos carros no frame atual
		rectangle(binariesGT,Rect(gts->xgt,gts->ygt,gts->widthgt,gts->heightgt),Scalar(255,255,255),CV_FILLED,8);

		for (; flag != 1;) {
			if (saidas->framesaida == gts->framegt) {
				flag = 1;
			} else {
				saidas++;
			}
		}
		flag = 0;
		
		for (; saidas->framesaida == gts->framegt; saidas++) {
			Mat binariesSaida(240, 320, CV_8UC1, Scalar(0,0,0));
			rectangle(binariesSaida,Rect(saidas->xsaida,saidas->ysaida,saidas->widthsaida,saidas->heightsaida),Scalar(255,255,255),CV_FILLED,8);
			imshow("GT",binariesGT);
			imshow("Saida",binariesSaida);
			getMatch(binariesGT, binariesSaida);
			cout << "Match: " << match[entradas-1] << "%" << endl;
			waitKey(0);
		}
		matches[gts->framegt-2][selecao] = getMajor();
		selecao++;
		gtDetected[gts->framegt-2]++;
		gts++;
		if (gts->framegt == contadorAuxiliar) {
			contadorAuxiliar++;
			selecao = 0;
		}
		entradas = 0;
	}

	printResultados();
	getFile();

	fclose(fp);
	fclose(fp2);
	return 0;
}

void getFile () {
	FILE * fpfinal = fopen ("MatchesResult.txt","w");
	int i, j;
	for (i = 0; i < 60; i++) {
		fprintf(fpfinal,"%d %d ",i+2,gtDetected[i]);
		for (j = 0; j < 30; j++) {
			if (j != 29) {
				fprintf(fpfinal,"%.01lf ",matches[i][j]);
			} else {
				fprintf(fpfinal,"%.01lf",matches[i][j]);
			}
		}
		if (i != 59) {
			fprintf(fpfinal,"\n");
		}
	}
}

void printResultados () {
	int i, j;
	printf("Legenda: Frame(Selecoes no GT) - Porcentagem de Hits para cada deteccao\n");
	for (i = 0; i < 60; i++) {
		printf("%d(%d) - ",i+2,gtDetected[i]);
		for (j = 0; j < 30; j++) {
			printf("%.01lf|",matches[i][j]);
		}
		printf("\n");
	}
}

double getMajor () {
	int i;
	double saida = 0;
	for (i = 0; i < entradas; i++) {
		if (match[i] > saida) {
			saida = match[i];
		}
	}
	return saida;
}

double getMatch (Mat gt, Mat saida) {
	int i, j;
	int area = 0;
	entradas++;
	double hit = 0;
	if (entradas == 1) {
		match = (double*) malloc (entradas*sizeof(double));
	} else {
		match = (double*) realloc (match, entradas*sizeof(double));
	}

	for (i = 0; i < gt.rows; i++) {
        for (j = 0; j < gt.cols; j++) {
            if (gt.at<uchar>(i,j) == 255) {
                area++;
            }
        }
    }

	for (i = 0; i < gt.rows; i++) {
        for (j = 0; j < gt.cols; j++) {
        	if (gt.at<uchar>(i,j) == 255) {
            	if (gt.at<uchar>(i,j) == saida.at<uchar>(i,j)) {
                	hit++;
            	}
            }
        }
    }

    match[entradas-1] = (100)*((double)hit/(double)area);
}
