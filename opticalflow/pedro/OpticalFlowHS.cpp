#include "include/HS.hpp"
#include "include/segmentation.hpp"

IplImage* imgROI( IplImage* );

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

    //  Img
    IplImage *current = 0, *next = 0, *Results = 0;   
    current =  cvQueryFrame( capture ); 

    current = imgROI( current );

    next = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);

    //  Gray-Img
    IplImage *curr_gray = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    IplImage *next_gray = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    cvCvtColor(current, curr_gray, CV_RGB2GRAY); 
    
    if(!(current))
    { 
        help();
        printf("\nCouldn`t start video capture\n");
        return -1;
    }   
      
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

        Mat segmented = flowSegmentation( current, Results );

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

