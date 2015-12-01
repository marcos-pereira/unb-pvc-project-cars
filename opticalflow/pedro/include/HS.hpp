#include "cv.h"  
#include "highgui.h"
#include <opencv2/legacy/legacy.hpp>  
#include <math.h>  
#include <stdio.h>  

using namespace cv;
using namespace std;


IplImage* visibleFlow( cv::Size, IplImage*, IplImage*, float );

IplImage* opticalFlowHS( IplImage*, IplImage*, float, int, float );