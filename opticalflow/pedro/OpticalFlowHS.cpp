#include "include/HS.hpp"
#include "include/segmentation.hpp"

IplImage* imgROI( IplImage* );

string getFileName( string );

void createNewFile( string );

void createWindows();

void destroyWindows();

void help()
{
    cout <<
        "\nThis program is designed to peform a segmentation based on Optical Flow \n"
        "it`s basic usage is\n"
        "\t     ./OpticalFlowHS <path/to/video/file>\n"
        "The parameters of the Optical flow can influence your results.\n"
        "To alter them, just open the src/segmentation.cpp file and change the values on #define:\n"
        "\t"     "LAMBDA    - wheighting factor of cvCalcOpticalFlowHS();\n"
        "\t"     "SMOOTH    - Kernel size and std deviation of Gaussian`s smooth operation;\n"
        "\t"     "THESH_VEL - Threshold value for velocities\n"
        "To end video capture, press 'esc'\n"
    << endl;
}

int main(int argc, char** argv)  
{   
    bool running = true;
    int frameCount = 0;

    //	Capture
    CvCapture* capture = cvCreateFileCapture( argv[1] );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FPS, 33 );

    //  File
    std::string filename;
    filename = getFileName( argv[1] );
    createNewFile( filename );

    //  Img
    IplImage *current = 0, *next = 0, *Results = 0;   
    current =  cvQueryFrame( capture ); 
    //  skip first frame
    current = cvQueryFrame( capture );
    frameCount++;

    if(!(current))
    { 
        help();
        cout << "\nCouldn`t start video capture" << endl;
        return -1;
    }

    current = imgROI( current );

    next = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);

    //  Gray-Img
    IplImage *curr_gray = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    IplImage *next_gray = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    cvCvtColor(current, curr_gray, CV_RGB2GRAY); 
      
    createWindows();

    while( running )
    {
        next =  cvQueryFrame( capture ); 

        if(!(next))
        {
            printf("Video ended\n");
            break;
        }

        frameCount++;
        next = imgROI( next );

        cvCvtColor( next, next_gray, CV_RGB2GRAY );
            
        Results = opticalFlowHS( curr_gray, next_gray, LAMBDA, SMOOTH, THRESH_VEL );

        Mat segmented = flowSegmentation( filename ,current, Results, frameCount );

        //  Show tracking  
        cvShowImage( "Raw Image", current );  
        cvSet( current, cvScalar(255, 255, 255), Results );
        cvShowImage( "Raw Flow", current );
        imshow( "Segmented Image", segmented );  
      
        //  Atualize
        current = cvCloneImage( next );
        curr_gray = cvCloneImage( next_gray );

        char c = cvWaitKey( 0 );  

        if( c == 27 )
            running = false;
    }
    cvReleaseCapture( &capture );
    // destroy windows  
    destroyWindows();
    // release memory  
    cvReleaseImage( &curr_gray ); 
    cvReleaseImage( &next_gray );  
    cvReleaseImage( &Results );  
  
    return 0;  
}  


void createWindows()
{
    cvNamedWindow( "Raw Image" );  
    cvNamedWindow( "Raw Flow"  );  
    namedWindow  ( "Dilation"  );
}

void destroyWindows()
{
    cvDestroyWindow( "Raw Image"  );  
    cvDestroyWindow( "Raw Flow"   );  
    cvDestroyWindow( "Dilatation" );
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

