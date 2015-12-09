#include "cv.h"  
#include "highgui.h"
#include <opencv2/legacy/legacy.hpp>  
#include <fstream>
#include <math.h>  

using namespace cv;
using namespace std;

//--Parameters--------------//

//-Optical-Flow------------//
#define LAMBDA      0.01
#define SMOOTH       9 
#define THRESH_VEL  0.7

//-Draw-Contours-----------//
#define MINAREA		250
#define MAXAREA		2000

//-Morphological-Ops-------//
#define DILATESIZE 	1

//--------------------------//

IplImage* imgROI( IplImage* );

string intToString(int number);

string getFileName( string );

void createNewFile( string );
