#include "include/HS.hpp"

#define LAMBDA      0.01
#define SMOOTH      9
#define THRESH_VEL  1

//Morph-Ops-vars---------------
int dilation_size = 0;
int const max_kernel_size = 21;

RNG rng(12345);
//-----------------------------

Mat drawContour( const Mat , Mat );

Mat flowDilate( const Mat );

Mat flowSegmentation( IplImage*, IplImage* );

void createWindows();

void destroyWindows();


void help()
{
    cout <<
        "\nThis program is designed to peform a segmentation based on Optical Flow \n"
        "it`s basic usage is\n"
            "\t./OpticalFlowHS <path/to/video/file>\n"
        "The parameters of the Optical flow can influence your results.\n"
        "To alter them, just open the .cpp file and change the values on #define:\n"
            "\tLAMBDA    - wheighting factor of cvCalcOpticalFlowHS();\n"
            "\tSMOOTH    - Kernel size and std deviation of Gaussian`s smooth operation;\n"
            "\tTHESH_VEL - Threshold value for velocities\n"
        "To end video capture, press 'esc'\n"
    << endl;
}

int main(int argc, char** argv)  
{  
    help();
    
    bool running = true;
    CvCapture* capture = cvCreateFileCapture( argv[1] );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FPS, 33 );

    //  Img
    IplImage *current = 0, *next = 0, *Results = 0;   
    current =  cvQueryFrame( capture ); 
    next = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    //  Gray-Img
    IplImage *curr_gray = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    IplImage *next_gray = cvCreateImage(cvGetSize(current),IPL_DEPTH_8U,1);
    cvCvtColor(current, curr_gray, CV_RGB2GRAY); 
    
    if(!(current))
    { 
        printf("Couldn`t start video capture\n");
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

        cvCvtColor( next, next_gray, CV_RGB2GRAY );
            
        Results = opticalFlowHS( curr_gray, next_gray, LAMBDA, SMOOTH, THRESH_VEL );

        Mat segmented = flowSegmentation( current, Results );

        //  Show tracking  
        cvShowImage( "Raw Image", current );  
        cvSet( current, cvScalar(255, 255, 255), Results );
        cvShowImage( "Raw Flow", current );
        imshow( "Segmented Image", segmented );  
      
        //  Atualize
        curr_gray = cvCloneImage( next_gray );

        char c = cvWaitKey( 1 );  

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
    
    // Create Dilation Trackbar
    createTrackbar( "Kernel size: ", "Dilation",
                  &dilation_size, max_kernel_size );
}

void destroyWindows()
{
    cvDestroyWindow( "Raw Image"  );  
    cvDestroyWindow( "Raw Flow"   );  
    cvDestroyWindow( "Dilatation" );
}

Mat drawContour( const Mat Original, Mat threshImage )
{
    //  Transform to Mat and copy it
    Mat aux(Original), segmented;
    aux.copyTo(segmented);

    int i;
    Rect contourRect;
    vector<vector<Point> > contours; // for storing contour
    vector<Vec4i> hierarchy;

    findContours( threshImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    //  Search through contours
    for( i = 0; i < contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        contourRect = boundingRect(contours[i]);
        rectangle( segmented, contourRect, color, 2 );
    }
    
    return segmented;
}

Mat flowDilate( const Mat input )
{
    Mat output;
    int dilation_type = MORPH_RECT;
    
    Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
    //  Apply the dilation operation
    dilate( input, output, element );
    imshow( "Dilation", output );
    return output;
}

Mat flowSegmentation( IplImage* original, IplImage* iplBin )
{
    Mat matOrig(original), matBin( iplBin );
    Mat mask = flowDilate( matBin );

    return (drawContour( matOrig, mask ));
}