#include "flowCore.hpp"

Mat drawContour( const Mat , Mat );

Mat drawContour( const string, const Mat , Mat, const int );

Mat flowDilate( const Mat );

Mat flowSegmentation(  IplImage*, IplImage* );

Mat flowSegmentation( string, IplImage*, IplImage*, int );