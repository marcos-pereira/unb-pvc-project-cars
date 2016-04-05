#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>



using namespace std;
using namespace cv;

int main( int argc, char** argv )
{  
    ofstream myfile;
    ostringstream path;
    string numChar;
    ostringstream convert;

    if (argc < 2)
    {
        printf("!!! Usage: ./program <filename>\n");
        return -1;
    }

    int mult = 5;
    myfile.open("bg4.txt");
    //path << "negative_samples/6/frame_";
    
    int cont;
    printf("* Filename: %s\n", argv[1]);  
    printf("Will split in %d x", mult); 

    CvCapture *capture = cvCaptureFromAVI(argv[1]);
    if(!capture) 
    {
        printf("!!! cvCaptureFromAVI failed (file not found?)\n");
        return -1; 
    }

    int result = mkdir("output2/", 0777);

    int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    printf("* FPS: %d\n", fps);

    IplImage* frame = NULL;
    int frame_number = 0;
    char key = 0;   

    while (key != 'q') 
    {
        for (cont = 0; cont <= mult+1 ; cont++){// get frame more captures to get 
            frame = cvQueryFrame(capture);  
        }


        if (!frame) 
        {
            printf("!!! cvQueryFrame failed: no frame\n");
            break;
        }       

        char filename[100];
        //char *path="/home/felipedalosto/UnBeatables/haar_cascade/tools/split_videos/output/";
        char *path="output2/";
        strcpy(filename, path);
        strcat(filename, "frame_");

        char frame_id[30];
        //itoa(frame_number, frame_id, 10);
        sprintf(frame_id,"%d",frame_number);
        //strcat(filename, "frame_");
        strcat(filename, frame_id);
        strcat(filename, ".jpg");
        
        printf("* Saving: %s\n", filename);

        myfile << "negative_samples/5/frame_";
        convert << frame_number;
        numChar = convert.str();

        if (frame_number < 100){
            if (frame_number < 10){
                myfile << "0";
            }
            myfile << "0";
        }
        myfile << numChar << ".jpg" << endl;
        convert.str("");

        if (!cvSaveImage(filename, frame))
        {
            printf("!!! cvSaveImage failed\n");
            break;
        }

        frame_number++;

        // quit when user press 'q'
        key = cvWaitKey(1000 / fps);
    }

    // free resources
    cvReleaseCapture(&capture);

    return 0;
}