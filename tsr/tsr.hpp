#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "../bp-nn/database.hpp"
#include "../bp-nn/neuralNet.hpp"

class TSR
{
public:
    TSR();

    std::pair<cv::Mat, std::vector<Sign_params_t>> sign_from_img(cv::Mat bgr_image, contours_t color);

private:
    Neural_net nn;
};
