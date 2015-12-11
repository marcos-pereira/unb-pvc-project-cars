//============================================================================
//  Universidade de Brasilia
//  Departamento de Ciencia da Computacao
//  Principios de Visao Computacional
//  Trabalho Final: Contador de Carros
//  Integrantes: Marcos, Felipe Dalosto, Maximillian, Felipe Spinola, Pedro, Bruno Donnici, Gustavo
//============================================================================

#include "HaarCarDetector.h"	

//----------------------------------------------------------------------------
HaarCarDetector::HaarCarDetector(std::string path_to_haar_base)
{
  cascade_ = (CvHaarClassifierCascade*) cvLoad(path_to_haar_base.c_str(), 0, 0, 0);
  storage_ = cvCreateMemStorage(0);

}

//----------------------------------------------------------------------------
HaarCarDetector::~HaarCarDetector()
{
}

//----------------------------------------------------------------------------
std::vector<std::vector<double>> HaarCarDetector::GetCarsRectangles(IplImage *img, int frame_number)
{
  // Function adapted to a class method
  // Reference: https://www.behance.net/gallery/Vehicle-Detection-Tracking-and-Counting/4057777
  // Based on:   
  // Oliveira, M.; Santos, V. Automatic Detection of Cars in Real Roads using Haar-like Features 
  // https://sites.google.com/site/andrewssobral/Automatic_Detection_of_Cars_in_Real_Roads_using_Haar-like_Features.pdf?attredirects=0 


  // cv::Mat implementation to debug ROI and also show original image
  cv::Mat img_mat(img);
  // Create ROI to desired area
  imshow("original frame", img_mat);
  // cv::Mat roi_image = img_mat(cv::Rect(img_mat.cols*1/2.6,img_mat.rows*1/3.5,img_mat.cols - img_mat.cols/2.6,img_mat.rows*2/3.1));
  // imshow("roi_image",roi_image);
  
  // IplImage ipl_img = img_mat;

  // Uncomment to find cars just on desired ROI
  //cvSetImageROI(img, cvRect(img->width*1/2.6, img->height*1/3.5, img->width - img->width*1/2.6, img->height*2/3.1));
  IplImage *img2 = cvCreateImage(cvGetSize(img),
                           img->depth,
                           img->nChannels);

  // std::cout << "img->width" << img->width << std::endl;

  cvCopy(img, img2, NULL);
  cvResetImageROI(img);

  CvSize img_size = cvGetSize(img2);
  CvSeq *object = cvHaarDetectObjects(
    img2,
    cascade_,
    storage_,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1.1, //2        //------------------MIN NEIGHBOURS	
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
    rectangles_properties.push_back(r->width);
    rectangles_properties.push_back(r->height);
    rectangles_properties.push_back(r->x + r->width/2);
    rectangles_properties.push_back(r->y + r->height/2);
    // rectangles_properties.push_back(r->width * r->height);    
    rectangles.push_back(rectangles_properties);    
    rectangles_properties.clear();
  }

  cv::Mat cars_rectangles(img2);
  // imwrite("output.jpg",cars_rectangles);

  cvShowImage("Carros", img2);
  cvWaitKey(0);

  return rectangles;
}