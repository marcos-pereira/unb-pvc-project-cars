#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <sstream>
#include <fstream>

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

std::vector<std::vector<double>> detect(IplImage *img, char** argv, int frame_number);

int main(int argc, char** argv)
{
  CvCapture *capture;
  IplImage  *frame;
  int input_resize_percent = 100;
  
  if(argc < 3)
  {
    std::cout << "Usage " << argv[0] << " path_to_cars3.xml path_to_video.avi" << std::endl;
    return 0;
  }

  if(argc == 4)
  {
    input_resize_percent = atoi(argv[3]);
    std::cout << "Resizing to: " << input_resize_percent << "%" << std::endl;
  }

  cascade = (CvHaarClassifierCascade*) cvLoad(argv[1], 0, 0, 0);
  storage = cvCreateMemStorage(0);
  capture = cvCaptureFromAVI(argv[2]);

  assert(cascade && storage && capture);

  //cvNamedWindow("video", 1);

  IplImage* frame1 = cvQueryFrame(capture);
  frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent)/100) , (int)((frame1->height*input_resize_percent)/100)), frame1->depth, frame1->nChannels);

  const int KEY_SPACE  = 32;
  const int KEY_ESC    = 27;

  // File variables
  std::ofstream detected_cars_datafile;
  std::string detected_cars_filename;
  detected_cars_filename = argv[2];
  detected_cars_filename.erase(0,detected_cars_filename.find_last_of("/")+1);
  detected_cars_filename += ".txt";
  std::cout << "cars file name: " << detected_cars_filename << std::endl;
  detected_cars_datafile.open (detected_cars_filename.c_str(), std::ios::out);

  std::vector<std::vector<double>> rectangles;

  int frame_number = 0;

  int key = 0;
  do
  {    
    frame1 = cvQueryFrame(capture);

    if(!frame1)
      break;

    cvResize(frame1, frame);

    rectangles = detect(frame, argv, frame_number);

    // write rectangles to file
    for(int ii = 0; ii < rectangles.size(); ii++)
    {
      for(int jj = 0; jj < rectangles[ii].size() ; jj++)
      {
        detected_cars_datafile << rectangles[ii][jj] << ";";
      }
      detected_cars_datafile << std::endl;
    }
    
    frame_number++;

    key = cvWaitKey(10);

    if(key == KEY_SPACE)
      key = cvWaitKey(0);

    if(key == KEY_ESC)
      break;

  }while(1);

  detected_cars_datafile.close();

  cvDestroyAllWindows();
  cvReleaseImage(&frame);
  cvReleaseCapture(&capture);
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseMemStorage(&storage);

  return 0;
}

std::vector<std::vector<double>> detect(IplImage *img, char** argv, int frame_number)
{

  // cv::Mat implementation to debug ROI and also show original image
  cv::Mat img_mat(img);
  // Create ROI to desired area
  imshow("original frame", img_mat);
  // cv::Mat roi_image = img_mat(cv::Rect(img_mat.cols*1/2.6,img_mat.rows*1/3.5,img_mat.cols - img_mat.cols/2.6,img_mat.rows*2/3.1));
  // imshow("roi_image",roi_image);
  
  // IplImage ipl_img = img_mat;

  cvSetImageROI(img, cvRect(img->width*1/2.6, img->height*1/3.5, img->width - img->width*1/2.6, img->height*2/3.1));
  IplImage *img2 = cvCreateImage(cvGetSize(img),
                           img->depth,
                           img->nChannels);

  // std::cout << "img->width" << img->width << std::endl;

  cvCopy(img, img2, NULL);
  cvResetImageROI(img);

  CvSize img_size = cvGetSize(img2);
  CvSeq *object = cvHaarDetectObjects(
    img2,
    cascade,
    storage,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1, //2        //------------------MIN NEIGHBOURS
    0, //CV_HAAR_DO_CANNY_PRUNING
    cvSize(0,0),//cvSize( 30,30), // ------MINSIZE
    img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
    );

  std::vector<std::vector<double>> rectangles;
  std::vector<double> rectangles_properties;

  std::cout << "Total: " << object->total << " cars" << std::endl;
  for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
  {
    CvRect *r = (CvRect*)cvGetSeqElem(object, i);
    cvRectangle(img2,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(255, 0, 0), 2, 8, 0);
    rectangles_properties.push_back(frame_number);
    rectangles_properties.push_back(r->x);
    rectangles_properties.push_back(r->y);
    rectangles_properties.push_back(r->x + r->width/2);
    rectangles_properties.push_back(r->y + r->height/2);
    rectangles_properties.push_back(r->width * r->height);    
    rectangles.push_back(rectangles_properties);    
    rectangles_properties.clear();
  }


  cvShowImage("ROI", img2);
  cvWaitKey(0);

  return rectangles;
}