#include "cv.h"  
#include "highgui.h"

using namespace cv;
using namespace std;

void help()
{
	cout << 
			"This is the program designed to help creating our Ground Truth data\n"
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

void onMouse( int event, int x, int y, int, void* );

int main(int argc, char* argv[])
{
	help();

	bool running = true;

	Mat frame;

	VideoCapture capture;
	capture.open( argv[1] );

	namedWindow( "Video Capture" );
	setMouseCallback( "Video Capture", onMouse, 0 );

	while( running )
	{
		capture.read( frame );

		imshow( "Video Capture", frame );

		char c;
		do
		{
			c = waitKey(0);

			if( c == 27 ) // 'esc'
				running = false;

		}while( c != 32 && running );
	}

	return 0;
}


void onMouse( int event, int x, int y, int, void* )
{
    if ( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);
        selection &= Rect(0, 0, 650, 500);

    }

    switch ( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        cout << selection << endl;
        break;
    }
}
