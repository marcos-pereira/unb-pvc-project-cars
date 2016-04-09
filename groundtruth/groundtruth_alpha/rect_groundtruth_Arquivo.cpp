#include "cv.h"  
#include "highgui.h"
#include <strings.h>
#include <string.h>

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
int counterFrame = 0;
FILE * fp;

void onMouse(int event, int x, int y, int, void *);
void writeFile ();

int main(int argc, char* argv[]) {
	help();

	bool running = true;
	char arquivoTxt[10];
	strcpy(arquivoTxt,argv[1]);
	arquivoTxt[1] = '_';
	strcat(arquivoTxt,".txt");

	cout << arquivoTxt << endl;

	fp = fopen (arquivoTxt,"w");
	fprintf(fp, "Arquivo: %s\n\n",argv[1]);

	
	VideoCapture capture;
	capture.open( argv[1] );

	namedWindow("Video Capture");
	setMouseCallback("Video Capture", onMouse, 0);

	while(running) {
		capture.read(frame);
		counterFrame++;
		fprintf(fp, "\n-----------------------\n");
		fprintf(fp, "Frame %d:\n",counterFrame);

		imshow("Video Capture", frame);
		char c;
		do {
			c = waitKey(0);

			if(c == 27) {// 'esc'
				running = false;
			}

		} while(c != 32 && running);
	}

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
    	rectangle(frame,selection,Scalar(0,0,255),1,8,0);
    	writeFile();
    	imshow("Video Capture", frame);
        selectObject = false;
        cout << selection << endl;
        break;
    }
}

void writeFile () {
	fprintf(fp,"X: %d Y: %d Width: %d Height: %d\n", selection.x,selection.y,selection.width,selection.height);
}
