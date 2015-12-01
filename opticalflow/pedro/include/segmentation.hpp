#include "HS.hpp"

//--Segmentation-Params-----//
//-Optical-Flow------------//
#define LAMBDA      0.01
#define SMOOTH      9
#define THRESH_VEL  .7
//-Draw-Contours-----------//
#define MINAREA		300
#define MAXAREA		1700
//-Morphological-Ops-------//
#define DILATESIZE 	3
//-------------------------//

Mat drawContour( const Mat , Mat );

Mat flowDilate( const Mat );

Mat flowSegmentation( IplImage*, IplImage* );