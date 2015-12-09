#include "../include/flowCore.hpp"

IplImage* imgROI( IplImage* orig )
{
    IplImage *img = cvCloneImage( orig );
    cvSetImageROI(
        img, cvRect(img->width*1/2.6,
        img->height*1/3.5,
        img->width - img->width*1/2.6,
        img->height*2/3.1)
        );
    IplImage *img2 = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
    img2 = cvCloneImage( img );
    cvResetImageROI(img);
    return img2;
}

string intToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

string getFileName( string videoName )
{
    string filename;

    filename = videoName;
    filename.erase( 0, filename.find_last_of("/")+1 );
    filename += ".txt";

    cout << "cars file name: " << filename << endl;

    return filename;
}

void createNewFile( string filename )
{
    ofstream dataFile;
    dataFile.open ( filename.c_str(), ios::out );
    dataFile.close ();
}