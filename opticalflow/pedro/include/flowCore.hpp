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
#define SMOOTH      9
#define THRESH_VEL  .7

//-Draw-Contours-----------//
#define MINAREA		250
#define MAXAREA		1700

//-Morphological-Ops-------//
#define DILATESIZE 	3

//--------------------------//

IplImage* imgROI( IplImage* );

string getFileName( string );

void createNewFile( string );