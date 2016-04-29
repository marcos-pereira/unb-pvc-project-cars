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

	std::string gt_txt_rectangles = argv[2];
	std::string negative_samples_filename; 
	std::string video_file_name = argv[1];

	//Variaveis de posicionamento
	int x, y, height, width, centerX, centerY, counterFramePlus;

	//Informacao do arquivo txt aberto
	cout << arquivoTxt << endl;

	fp = fopen (gt_txt_rectangles.c_str(),"r");
	
	//Descarte do primeiro frame
	VideoCapture capture;
	capture.open( argv[1] );
	capture.read(frame);


	namedWindow("Video Capture");

	//Variaveis de contagem para auxiliar no desenho dos retangulos
	counterFrame = 2;
	counterFramePlus = 3;

	// ROI needed matrices
	Mat original_frame;
	Mat original_frame_to_draw_black_rectangles;
	Mat roi_frame;
	std::ostringstream frame_counter_ss;	

	while(running && (!feof(fp))) {
		if (capture.read(frame)) {
			//Estrutura condicional que verifica se o frame atual eh diferente do segundo, pois se for realiza o desenho do ultimo
			//retangulo adquirido no procedimento de fscanf
			if (counterFrame != 2) {
				rectangle(frame,Rect(x,y,width,height),Scalar(0,0,0),CV_FILLED,8,0);				    	   	
			}
			//Estrutura de laço de repeticao para varrer o arquivo .txt capturando as informacoes de posicionamento
			//para realizar o desenho dos retangulos nos frames
			while (counterFrame != counterFramePlus && (!feof(fp))) {
				fscanf(fp,"%d;%d;%d;%d;%d;%d;%d",&counterFrame, &x, &y, &width, &height, &centerX, &centerY);
				if (counterFrame != counterFramePlus) {
					rectangle(frame,Rect(x,y,width,height),Scalar(0,0,0),CV_FILLED,8,0);
				}
			}

			frame_counter_ss << counterFrame; // used to convert from int to string

			original_frame_to_draw_black_rectangles = frame.clone();

			//Incremento da variavel que indica o proximo valor de frame (auxiliar de contagem)
			counterFramePlus = counterFramePlus + 1;

			// Draw rectangle of ROI
			rectangle(frame, Rect(frame.cols*1/2.6,frame.rows*1/3.5,frame.cols - frame.cols/2.6,frame.rows*2/3.1),Scalar(0,0,255),1,8,0);			
			// Set rectangle that defines ROI of image
			cv::Rect rectangle_roi( frame.cols*1/2.6,frame.rows*1/3.5,frame.cols - frame.cols/2.6,frame.rows*2/3.1);					

			// Store original frame with a red rectangle around ROI
			original_frame = frame.clone();		
			
			// Show original image with ROI rectangle
			imshow("Original frame",original_frame);

			// Get original image without red rectangle to use for drawing black rectangle and generate negative samples
			roi_frame = original_frame_to_draw_black_rectangles.clone();						

			// Get image ROI
			roi_frame = roi_frame( rectangle_roi );			

			imshow("ROI frame",roi_frame);
			
			negative_samples_filename = video_file_name + "_" + "negative_sample" + frame_counter_ss.str() + "_" + ".png";

			//Exibicao do frame desenhado
			imshow("Video Capture", frame);
			imwrite(negative_samples_filename, roi_frame);		
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
