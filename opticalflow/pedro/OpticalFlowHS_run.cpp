#include "include/HS.hpp"
#include "include/segmentation.hpp"

void createWindows();

void destroyWindows();

string videoname[21] = 
{
	"../../videos\ renomeados/n1.avi",
    "../../videos\ renomeados/n2.avi",
    "../../videos\ renomeados/n3.avi",
    "../../videos\ renomeados/n4.avi",
    "../../videos\ renomeados/n5.avi",
    "../../videos\ renomeados/n6.avi",
    "../../videos\ renomeados/n7.avi",
    "../../videos\ renomeados/n8.avi",
    "../../videos\ renomeados/n9.avi",
    "../../videos\ renomeados/n10.avi",
    "../../videos\ renomeados/n11.avi",
    "../../videos\ renomeados/n12.avi",
    "../../videos\ renomeados/n13.avi",
    "../../videos\ renomeados/n14.avi",
    "../../videos\ renomeados/n15.avi",
    "../../videos\ renomeados/n16.avi",
    "../../videos\ renomeados/n17.avi",
};


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
    char c = 0;
    for( int i = 0; i < 17 && c != 27 ; i++ )
    {
    	bool running = true;
    	int frameCount = 0;

    	//	Capture
    	CvCapture* capture = cvCreateFileCapture( videoname[i].c_str() );
    	cvSetCaptureProperty( capture, CV_CAP_PROP_FPS, 33 );

    	//  File
    	std::string filename;
    	filename = getFileName( videoname[i] );
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

//    	current = imgROI( current );

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
//      	next = imgROI( next );

        	cvCvtColor( next, next_gray, CV_RGB2GRAY );
        	    
        	Results = opticalFlowHS( curr_gray, next_gray, LAMBDA, SMOOTH, THRESH_VEL );

        	Mat segmented = flowSegmentation( filename, current, Results, frameCount );

        	//  Show tracking  
        	cvShowImage( "Raw Image", current );  
        	cvSet( current, cvScalar(255, 255, 255), Results );
        	cvShowImage( "Raw Flow", current );
        	imshow( "Segmented Image", segmented );  
      
        	//  Atualize
        	//current = cvCloneImage( next );
        	curr_gray = cvCloneImage( next_gray );

        	c = cvWaitKey( 1 );  

        	if( c == 27 )
            	running = false;
    	}
    	cvReleaseCapture( &capture );
    	// release memory  
    	cvReleaseImage( &curr_gray ); 
    	cvReleaseImage( &next_gray );  
    	cvReleaseImage( &Results );  
    }
    // destroy windows  
    	destroyWindows();
    	
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
