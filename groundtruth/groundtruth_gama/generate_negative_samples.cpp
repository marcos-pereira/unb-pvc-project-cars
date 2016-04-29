//Declaracao das bibliotecas
#include "cv.h"  
#include "highgui.h"
#include <strings.h>
#include <string.h>
#include <string>

using namespace cv;
using namespace std;

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
bool selectObject = false;
Point origin;
Rect selection;
Mat frame;
Mat roi_frame;
Mat original_frame;
Mat ROI;
int counterFrame = 0;
int totalFrames = 0;
FILE * fp;

string negative_samples_filename_g;

void onMouse(int event, int x, int y, int, void *);
void writeFile ();


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

	cout << arquivoTxt << endl;

	fp = fopen (arquivoTxt,"w");
	
	VideoCapture capture, capture2;
	capture.open( argv[1] );

	//Contagem dos frames
	while (capture.read(frame)) {
		totalFrames++;
	}

	cout << "Numero de frames: " << totalFrames << endl;

	//Reabertura do video para selecao dos carros
	capture2.open( argv[1] );
	//Descarte do primeiro frame
	capture2.read(frame);
	counterFrame++;

	string video_file_name = argv[1];
	string negative_samples_file;
	std::ostringstream frame_counter_ss;	
	string frame_number;


	// Create and set window size to make drawing easier
	namedWindow("ROI frame drawing image",WINDOW_NORMAL);
	resizeWindow("ROI frame drawing image", 850, 900);
	// namedWindow("ROI frame",WINDOW_NORMAL);
	// resizeWindow("ROI frame", 850, 900);
	
	setMouseCallback("ROI frame drawing image", onMouse, 0);

	//Estrutura de laco de repeticao para executar ate o penultimo frame, visto que a maioria dos ultimos frames
	//estao danificados
	while(running && counterFrame != (totalFrames-1)) {
		if (capture2.read(frame)) {
			counterFrame++;			

			frame_counter_ss << counterFrame;			

			negative_samples_file = "negative_sample" + frame_counter_ss.str() + "_" + video_file_name + ".png";

			negative_samples_filename_g = negative_samples_file;

			Mat original_frame_to_draw_black_rectangles = frame.clone();

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
			// imshow("ROI frame",roi_frame);
			
			frame = roi_frame.clone();			
			imshow("ROI frame drawing image",frame);				
			char c;
			do {
				imwrite(negative_samples_filename_g, frame);
				negative_samples_file.clear();
				frame_counter_ss.str(std::string());
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


void onMouse(int event, int x, int y, int, void *){
    if (selectObject) {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);
        selection &= Rect(0, 0, 650, 500);
    }

    switch (event) {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
    	// Draw black rectangle over car
    	rectangle(frame,selection,Scalar(0,0,0),CV_FILLED,8,0);    	
    	rectangle(roi_frame,selection,Scalar(0,0,0),CV_FILLED,8,0);    	
    	writeFile();
    	imshow("ROI frame drawing image", frame);    	
    	imwrite(negative_samples_filename_g, frame);		
        selectObject = false;
        cout << selection << endl;
        break;
    }
}

//Funcao 'writeFile' para gerar o arquivo .txt com as informacoes de posicionamento dos retangulos sobre os carros
void writeFile () {
	int centerX = (int)((selection.x+selection.width)/2);
	int centerY = (int)((selection.y+selection.height)/2);
	if (counterFrame != (totalFrames-1)) {
		fprintf(fp,"%d;%d;%d;%d;%d;%d;%d\n", counterFrame,selection.x,selection.y,selection.width,selection.height,centerX,centerY);
	} else {
		fprintf(fp,"%d;%d;%d;%d;%d;%d;%d", counterFrame,selection.x,selection.y,selection.width,selection.height,centerX,centerY);
	}
}
