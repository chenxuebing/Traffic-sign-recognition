#include <iostream>
#include <opencv2/opencv.hpp>

#include "tsr.hpp"

int main(int argc, char** argv)
{
    TSR tsr;

    cv::Mat in_img = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

    std::pair<cv::Mat, std::vector<Sign_params_t>> out_pair;

    out_pair = tsr.sign_from_img(in_img, red_c);

    cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);
    cv::imshow("frame", out_pair.first);

    cv::waitKey(0);

    // out_pair = tsr.sign_from_img(in_img, yellow_c);

    // cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);
    // cv::imshow("frame", out_pair.first);
    // cv::waitKey(0);

}