#ifndef TRAFFIC_SIGN_RECOGNITION_H
#define TRAFFIC_SIGN_RECOGNITION_H

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>

cv::Mat tsr(cv::Mat frameGray,int upsample_amount = 0);

#endif // TRAFFIC_SIGN_RECOGNITION_H
