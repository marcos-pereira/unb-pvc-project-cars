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

using namespace std;
using namespace cv;

string filename[21] = {"cctv052x2004080516x01638.avi",
    "cctv052x2004080516x01639.avi",
    "cctv052x2004080516x01640.avi",
    "cctv052x2004080516x01641.avi",
    "cctv052x2004080516x01642.avi",
    "cctv052x2004080516x01643.avi",
    "cctv052x2004080516x01644.avi",
    "cctv052x2004080516x01645.avi",
    "cctv052x2004080516x01646.avi",
    "cctv052x2004080516x01647.avi",
    "cctv052x2004080516x01648.avi",
    "cctv052x2004080516x01649.avi",
    "cctv052x2004080516x01650.avi",
    "cctv052x2004080517x01652.avi",
    "cctv052x2004080517x01653.avi",
    "cctv052x2004080517x01654.avi",
    "cctv052x2004080517x01655.avi",
    "cctv052x2004080517x01656.avi",
    "cctv052x2004080517x01657.avi",
    "cctv052x2004080517x01658.avi",
};

int main(int argc, char **args) {
    
    Mat firstFrame, newFrame, gray, frameDelta, thresh;
    
    firstFrame = imread("firstFrame2.png",1);
    resize(firstFrame, firstFrame, cvSize(500, 375));
    cvtColor(firstFrame, firstFrame, COLOR_BGR2GRAY);
    GaussianBlur(firstFrame, firstFrame, cvSize(21, 21), 0);
    
    for (int i = 0; i < 20; i++) {
        
        VideoCapture cap(filename[i]);
        
        while(1) {
            cap >> newFrame;
            
            if (newFrame.empty()) {
                break;
            }
            
            resize(newFrame, newFrame, cvSize(500, 375));
            cvtColor(newFrame, gray, COLOR_BGR2GRAY);
            GaussianBlur(gray, gray, cvSize(21, 21), 0);
            
            if (newFrame.empty()) {
                newFrame = gray;
            }
            
            absdiff(firstFrame, gray, frameDelta);
            threshold(frameDelta, thresh, 40, 255, THRESH_BINARY);
            dilate(thresh, thresh, 4);
            
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

            }
            
            imshow("Feed", newFrame);
            moveWindow("Feed", 0, 50);
            imshow("Thresh", thresh);
            moveWindow("Thresh", 500, 50);
            imshow("Frame Delta", frameDelta);
            moveWindow("Frame Delta", 250, 425);
            
            waitKey(1);
            
        }
    }
    destroyAllWindows();
//    waitKey(0);
    
    return 0;
    
}