//
//  main.cpp
//  OpenCvFinal
//
//  Created by Felipe Ramos on 16/11/15.
//  Copyright © 2015 Felipe Ramos. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

//string filename[21] = {
//    "cctv052x2004080516x01638.avi",
//    "cctv052x2004080516x01639.avi",
//    "cctv052x2004080516x01640.avi",
//    "cctv052x2004080516x01641.avi",
//    "cctv052x2004080516x01642.avi",
//    "cctv052x2004080516x01643.avi",
//    "cctv052x2004080516x01644.avi",
//    "cctv052x2004080516x01645.avi",
//    "cctv052x2004080516x01646.avi",
//    "cctv052x2004080516x01647.avi",
//    "cctv052x2004080516x01648.avi",
//    "cctv052x2004080516x01649.avi",
//    "cctv052x2004080516x01650.avi",
//    "cctv052x2004080517x01652.avi",
//    "cctv052x2004080517x01653.avi",
//    "cctv052x2004080517x01654.avi",
//    "cctv052x2004080517x01655.avi",
//    "cctv052x2004080517x01656.avi",
//    "cctv052x2004080517x01657.avi",
//    "cctv052x2004080517x01658.avi",
//};

string filename[18] = {
    "n1.avi",
    "n2.avi",
    "n3.avi",
    "n4.avi",
    "n5.avi",
    "n6.avi",
    "n7.avi",
    "n8.avi",
    "n9.avi",
    "n10.avi",
    "n11.avi",
    "n12.avi",
    "n13.avi",
    "n14.avi",
    "n15.avi",
    "n16.avi",
    "n17.avi",
};

int main(int argc, char **args) {
    
    FILE *fp;
    
    string txtFileName;
    const char *constCharFileName;
    
    Mat firstFrame, newFrame, gray, frameDelta, thresh;
    
    int counterFrame, totalFrames = 0;
    
    firstFrame = imread("firstFrame2.png",1);
    resize(firstFrame, firstFrame, cvSize(500, 375));
    firstFrame = firstFrame(cv::Rect(firstFrame.cols*1/2.6,firstFrame.rows*1/3.5,firstFrame.cols - firstFrame.cols/2.6,firstFrame.rows*2/3.1));
    cvtColor(firstFrame, firstFrame, COLOR_BGR2GRAY);
    GaussianBlur(firstFrame, firstFrame, cvSize(21, 21), 0);
    
    for (int i = 0; i < 17; i++) {
    
        VideoCapture cap(filename[i]);
        
        txtFileName = filename[i].append(".txt");
        
        constCharFileName = txtFileName.c_str();
        
        fp = fopen(constCharFileName, "w");
        
        counterFrame = 0;
        
        totalFrames = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
        
        while(1) {
            cap >> newFrame;
            
            if (newFrame.empty()) {
                break;
            }
            
            counterFrame++;
            
            resize(newFrame, newFrame, cvSize(500, 375));
            newFrame = newFrame(cv::Rect(newFrame.cols*1/2.6,newFrame.rows*1/3.5,newFrame.cols - newFrame.cols/2.6,newFrame.rows*2/3.1));
            cvtColor(newFrame, gray, COLOR_BGR2GRAY);
            GaussianBlur(gray, gray, cvSize(21, 21), 0);
            
            if (newFrame.empty()) {
                newFrame = gray;
            }
            
            absdiff(firstFrame, gray, frameDelta);
            threshold(frameDelta, thresh, 50, 255, THRESH_BINARY);
            dilate(thresh, thresh, 10);
            
            vector<vector<Point> > newc;
            findContours(thresh, newc, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            for (int i = 0; i < newc.size(); i++) {
                double a = contourArea(newc[i]);
                if (a < 100) {
                    continue;
                }
                if (a > 3000) {
                    continue;
                }
                Rect r = boundingRect(newc[i]);
                Point pt1 = cvPoint(r.x, r.y);
                Point pt2 = cvPoint(r.x + r.width, r.y + r.height);
                rectangle(newFrame, pt1, pt2, Scalar(0,255,0),2);
                
                if (counterFrame == 1) {
                   
                } else if (counterFrame == totalFrames) {
                
                } else {
                    int centerX = (int)(r.x + r.width)/2;
                    int centerY = (int)(r.y + r.height)/2;
                    fprintf(fp, "%d;%d;%d;%d;%d;%d;%d\n",counterFrame,r.x,r.y,r.width,r.height,centerX,centerY);
                }
            }
            
            
            
            imshow("Feed", newFrame);
            moveWindow("Feed", 0, 50);
            imshow("Thresh", thresh);
            moveWindow("Thresh", 500, 50);
            imshow("Frame Delta", frameDelta);
            moveWindow("Frame Delta", 250, 425);
            
            waitKey(1);
            
        }
        
        fclose(fp);
    }
    destroyAllWindows();
//    waitKey(0);
    
    return 0;
    
}