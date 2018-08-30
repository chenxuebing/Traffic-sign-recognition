#include <iostream>
#include "traffic_sign_recognition.h"

// g++ test_main.cpp traffic_sign_recognition.cpp `pkg-config --cflags --libs opencv` `pkg-config --libs --cflags dlib-1` -lpthread -lX11 -llapack -std=c++11

int main(int argc, char** argv)
{
	cv::Mat img_rgb  = cv::imread(argv[1]);

	cv::Mat img_gray;

	cv::cvtColor(img_rgb, img_gray, CV_BGR2GRAY);

	cv::Mat filter(img_gray.rows, img_gray.cols, CV_8UC3, cv::Scalar(0, 0, 0));

	tld(img_gray, filter);

	cv::Mat res;

	cv::addWeighted(filter, 1.0, img_rgb, 1.0, 0, res);

	cv::imshow("test", res);
	cv::waitKey(0);
}
