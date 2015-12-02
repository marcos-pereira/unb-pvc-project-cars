#include "../include/segmentation.hpp"


Mat drawContour( const Mat Original, Mat threshImage )
{
    RNG rng(12345);
    //  Transform to Mat and copy it
    Mat aux(Original), segmented;
    aux.copyTo(segmented);

    int i, validCount = 0;
    Rect contourRect;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( threshImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    //  Search through contours
    for( i = 0; i < contours.size(); i++ )
    {
        double area = contourArea( contours[i] );
        if( area > MINAREA )
        	if( area < MAXAREA )
        	{
        		Scalar color = Scalar( rng.uniform(100, 200), rng.uniform(100,200), rng.uniform(100,200) );
        		contourRect = boundingRect( contours[i] );
        		rectangle( segmented, contourRect, color, 2 );
        	}
    }

    return segmented;
}

Mat flowDilate( const Mat input )
{
    Mat output;
    int dilation_type = MORPH_RECT;
    int dilation_size = DILATESIZE;

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

    return ( drawContour( matOrig, mask ) );
}

//--Write-to-File------------------------------------------------------//

Mat drawContour( string Filename, Mat Original, Mat threshImage, int frameCount )
{
    RNG rng(12345);
    
    //	File
    ofstream dataFile;
    dataFile.open ( Filename.c_str(), ios::app );

    //  Transform to Mat and copy it
    Mat aux(Original), segmented;
    aux.copyTo(segmented);

    int i;
    Rect contourRect;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( threshImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    //  Search through contours
    for( i = 0; i < contours.size(); i++ )
    {
        double area = contourArea( contours[i] );
        if( area > MINAREA )
        	if( area < MAXAREA )
        	{
        		Scalar color = Scalar( rng.uniform(100, 200), rng.uniform(100,200), rng.uniform(100,200) );
        		contourRect = boundingRect( contours[i] );
        		rectangle( segmented, contourRect, color, 2 );
        		
        		dataFile << frameCount << ";" 
        				 << contourRect.x   << ";" 
        				 << contourRect.y   << ";" 
        				 << contourRect.width   << ";" 
        				 << contourRect.height  << ";"
                         << (contourRect.x + contourRect.width)/2   << ";"
                         << (contourRect.y + contourRect.height)/2  << ";"
        				 << endl;
        	}
    }

    return segmented;
}

Mat flowSegmentation(  string Filename, IplImage* original, IplImage* iplBin, int frameCount )
{
    Mat matOrig(original), matBin( iplBin );
    Mat mask = flowDilate( matBin );

    return ( drawContour( Filename, matOrig, mask, frameCount ) );
}

//--------------------------------------------------------------------//