#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace cv;

RNG rng(12345);

void check_program_arguments(int argc) {
    if(argc != 2) {
        std::cout << "Error! Program usage:" << std::endl;
        std::cout << "./circle_detect image_circles_path" << std::endl;
        std::exit(-1);
    }   
}

void check_if_image_exist(const cv::Mat &img, const std::string &path) {
    if(img.empty()) {
        std::cout << "Error! Unable to load image: " << path << std::endl;
        std::exit(-1);
    }   
}


int main(int argc, char **argv) {
    // Usage: ./circle_detect image_circles_path
    check_program_arguments(argc);
    
    // Load input image
    std::string path_image{argv[1]};
    cv::Mat bgr_image = cv::imread(path_image);

    // Check if the image can be loaded
    check_if_image_exist(bgr_image, path_image);

    cv::Mat orig_image = bgr_image.clone();

    cv::medianBlur(bgr_image, bgr_image, 3);

    // Convert input image to HSV
    cv::Mat hsv_image;
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

    // Threshold the HSV image, keep only the red pixels
    cv::Mat lower_red_hue_range;
    cv::Mat upper_red_hue_range;
    cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

    // Combine the above two images
    cv::Mat red_hue_image;
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

    cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);



    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;

    findContours(red_hue_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Rect bounding_rect = boundingRect(contours[i]);
        rectangle(bgr_image, bounding_rect, Scalar(0,255,0),2,8,0);
    }

    cv::namedWindow("Combined threshold images", cv::WINDOW_AUTOSIZE);
    cv::imshow("Combined threshold images", bgr_image);

    cv::waitKey(0);

    
    return 0;
}
