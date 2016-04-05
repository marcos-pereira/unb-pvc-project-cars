/*
USAGE 

The code is ready to resize and save the names of the images. The execution must be on the folder of the samples, that must be named as "sample_XXXX".

*/


#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <gif_lib.h>
#include <string>
#include <vector>
#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
using namespace cv;

Mat resizeImage (Mat inputImage){

    Mat outputImage;
    Size actual_size, new_size;

    if (inputImage.rows > inputImage.cols){

        new_size = Size(inputImage.cols,inputImage.cols);
        //cout << inputImage.cols;     
    }
    else {
        new_size = Size(inputImage.rows,inputImage.rows); 
        //cout << inputImage.rows;   
    }
    if (new_size.height < 20){
        new_size = Size(20,20);
    }
    resize(inputImage, outputImage, new_size);

    return outputImage;

}

int main( int argc, char* argv[] )
{  
    
    Mat outputImage, inputImage;
    string inputPath, inputFolder, outputPath;

    string numChar, sizeChar;
    ostringstream convert; 
    ofstream myfile;

    int size;
    

    if (argc < 2)
    {
        printf("!!! Usage: ./program <inputFolder>\n");
        return -1;
    }
    int result = mkdir("output/", 0777);
    inputFolder= argv[1];

    cout << inputFolder << endl;


    myfile.open("pos_3.info");

    for (int num = 1;num<1000;num++){
        inputPath = inputFolder + string("sample_");

        if (num < 100){
            if (num < 10){
                inputPath = inputPath + string ("0");
            }
            //inputPath = inputPath + string ("0");
        }

        convert << num; 
        numChar = convert.str();
        inputPath = inputPath + numChar;
        cout << inputPath << endl;

        
        inputImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
        cout << "can read";
        imshow("w",inputImage);

        //waitKey();
        
        outputImage = resizeImage(inputImage);
        

        outputPath = string("output/sample_resized_") + numChar + string(".png");

        size = outputImage.rows;
        imwrite (outputPath, outputImage);
        convert.str("");

        convert << size;
        sizeChar = convert.str();
        convert.str("");


        myfile << "image_files/resized_positive_samples/3/sample_resized_" + numChar +  string(".png");
        myfile << " 1 0 0 " << sizeChar << " " << sizeChar << endl;
      
    } 

    return 0;
}