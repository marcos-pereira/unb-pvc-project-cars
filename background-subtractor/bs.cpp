//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include "../cvBlob/cvblob.h"

using namespace cv;
using namespace std;

//global variables
Mat frame; //current frame
Mat fgMaskMOG; //fg mask generated by MOG method
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
int keyboard;
int numFrame = 0;

//function declarations
void help();
void processVideo(char* videoFilename);

void help()
{
    cout
        << "--------------------------------------------------------------------------"  << endl
        << "This program shows how to use background subtraction methods provided by "   << endl
        << " OpenCV. You can process both videos."                                       << endl
        << endl
        << "Usage:"                                                                      << endl
        << "./bs {-vid <video filename>"                                                 << endl
        << "for example: ./bs video.avi"                                                 << endl
        << "--------------------------------------------------------------------------"  << endl
        << endl;
}

int main(int argc, char* argv[])
{
    //print help information
    help();

    //check for the input parameter correctness
    if(argc != 2) {
        cerr <<"Incorret input list" << endl;
        cerr <<"exiting..." << endl;
        return EXIT_FAILURE;
    }

    //create GUI windows
    namedWindow("Frame");
    namedWindow("FG Mask MOG");
    namedWindow("FG Mask MOG 2");

    //create Background Subtractor objects
   //NOTE HERE!!!!
    pMOG= new BackgroundSubtractorMOG(); //MOG approach
    pMOG2 = new BackgroundSubtractorMOG2(); //MOG2 approach

    if(argv[1] != NULL) {
        //input data coming from a video
        processVideo(argv[1]);
    }
    else {
        //error in reading input parameters
        cerr <<"Please, check the input parameters." << endl;
        cerr <<"Exiting..." << endl;
        return EXIT_FAILURE;
    }
    //destroy GUI windows
    destroyAllWindows();
    return EXIT_SUCCESS;
}

void processVideo(char* videoFilename) {
    //create the capture object
    VideoCapture capture(videoFilename);
    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file: " << videoFilename << endl;
        exit(EXIT_FAILURE);
    }
    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){

        //read the current frame
        if(!capture.read(frame)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            exit(EXIT_FAILURE);
        }
        //update the background model
           //AND HERE!!!
        pMOG->operator()(frame, fgMaskMOG);
        pMOG2->operator()(frame, fgMaskMOG2);
        //get the frame number and write it on the current frame
        
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20), cv::Scalar(255,255,255), -1);
        
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask MOG", fgMaskMOG);
        imshow("FG Mask MOG 2", fgMaskMOG2);
        //get the input from the keyboard
        keyboard = waitKey( 0 );

        
    }
    //delete capture object
    capture.release();
}