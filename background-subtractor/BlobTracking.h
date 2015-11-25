// Author: Andrews Sobral
// Reference: https://github.com/andrewssobral/simple_vehicle_counting

#pragma once

#include <iostream>
#include "cv.h"
#include "highgui.h"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>

#include <cvblob.h>

class BlobTracking
{
private:
  bool firstTime;
  int minArea;
  int maxArea;
  
  bool debugTrack;
  bool debugBlob;
  bool showBlobMask;
  bool showOutput;

  cvb::CvTracks tracks;
  void saveConfig();
  void loadConfig();

public:
  BlobTracking();
  ~BlobTracking();

  void process(const cv::Mat &img_input, const cv::Mat &img_mask, cv::Mat &img_output);
  const cvb::CvTracks getTracks();
};

