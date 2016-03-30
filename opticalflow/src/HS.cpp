#include "../include/HS.hpp"



IplImage* visibleFlow( cv::Size inputSize, IplImage* velx, IplImage* vely, float threshVel )
{
    //  Output matrix
    IplImage* imgOut = cvCreateImage( inputSize, IPL_DEPTH_8U, 1 );
    
    //  Draw Optical FLow
    cvZero( imgOut );  
    int step = 1;  

    //  Iterate through velocities matrixes
    for( int row = 0; row < imgOut->height; row += step ) 
    {  
        //Position volcity`s pointers according to row`s positions
        float* ptVelx = (float*) ( velx->imageData + row * velx->widthStep );  
        float* ptVely = (float*) ( vely->imageData + row * vely->widthStep );  

        //Iterate through each row`s collumns
        for( int col = 0; col < imgOut->width; col += step )
        {  
        //  Spot a interesting point -- Thresh it
            if( ptVelx[col] > threshVel || ptVely[col] > threshVel )
            {  
            //  Normalize
                ptVelx[col] = 1;
                ptVely[col] = 1;
            //  Fill whith circles
                cvCircle(  
                    imgOut,  
                    cvPoint( col + ptVelx[col]/2, row+ptVely[col]/2 ),  
                    2,  
                    cvScalar( 255, 255, 255 ),  
                    -1 
                );  
            }
            else
            {
                ptVelx[col] = 0;
                ptVely[col] = 0;
            }  
        }  
    }
    return imgOut;
}


IplImage* opticalFlowHS( IplImage* imgPrev, IplImage* imgCurr, float Lambda, int Smooth, float threshVel )
{
    //  Create 32 Float matrices to store velocities
    IplImage* velx = cvCreateImage(cvGetSize(imgPrev),IPL_DEPTH_32F,1);  
    IplImage* vely = cvCreateImage(cvGetSize(imgPrev),IPL_DEPTH_32F,1);  
  
    // Call the 1.x Horn and Schunck algorithm  
    cvCalcOpticalFlowHS(   
        imgPrev,                                     //Call previous
        imgCurr,                                     //Call current
        0,                                           //Flag -> use previous
        velx,                                        //Stores X velocity
        vely,                                        //Stores Y velocity
        Lambda,                                      //Lambda(smoothness)                                         
        cvTermCriteria(//Recursive stop criteria
            CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,      //Type
            imgPrev->width,                          //Max count
            1e-6                                     //Accuracy( limits criteria )
        )  
    );
    //  Low pass Gaussian Filter
    cvSmooth(   velx,
                velx,
                CV_GAUSSIAN,
                Smooth,
                Smooth,
                Smooth 
            );
    cvSmooth(   vely,
                vely,
                CV_GAUSSIAN,
                Smooth,
                Smooth,
                Smooth 
            );
    //  Make it visible
    IplImage* imgOut = visibleFlow( cvGetSize(imgPrev), velx, vely, threshVel );

    return imgOut;  
}
