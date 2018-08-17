#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

// g++ detect_red.cpp `pkg-config --cflags --libs opencv` -std=c++11

namespace {

bool warning_sign(std::vector<cv::Point> out, std::vector<cv::Point> in)
{
    if (out[1].y > out[0].y || out[1].y > out[2].y || in[1].y > in[0].y || in[1].y > in[2].y)
        return false;

    if (out[0].y < in[0].y || out[2].y < in[2].y || out[1].y > in[1].y)
        return false;

    if (out[0].x > in[0].x || out[2].x < in[2].x)
        return false;

    std::cout << "Warning sign datected!" << std::endl;

    return true;
}

bool give_way_sign(std::vector<cv::Point> out, std::vector<cv::Point> in)
{
    if (out[1].y < out[0].y || out[1].y < out[2].y || in[1].y < in[0].y || in[1].y < in[2].y)
        return false;

    if (out[0].y > in[0].y || out[2].y > in[2].y || out[1].y < in[1].y)
        return false;

    if (out[0].x > in[0].x || out[2].x < in[2].x)
        return false;

    std::cout << "Give way datected!" << std::endl;
    
    return true;
}

bool point_sort(cv::Point p1, cv::Point p2)
{
    return p1.x < p2.x;
}

void find_red_triangles(cv::Mat& img, const std::vector<std::vector<cv::Point>>& contours)
{
    std::vector<cv::Point> approx_sign_x, approx_sign_y;

    for (std::size_t x = 0; x < contours.size(); x++)
    {
        cv::approxPolyDP(contours[x], approx_sign_x, cv::arcLength(cv::Mat(contours[x]), true) * 0.05, true);

        if (approx_sign_x.size() == 3)
        {
            for (std::size_t y = x + 1; y < contours.size(); y++)
            {
                cv::approxPolyDP(contours[y], approx_sign_y, cv::arcLength(cv::Mat(contours[y]), true) * 0.05, true);

                if (approx_sign_y.size() == 3)
                {

                    std::sort(approx_sign_x.begin(), approx_sign_x.end(), point_sort);
                    std::sort(approx_sign_y.begin(), approx_sign_y.end(), point_sort);
                    
                    if (give_way_sign(approx_sign_x, approx_sign_y) == true)
                    {
                        cv::drawContours(img, contours, x, cv::Scalar(125, 255, 0), 2);
                    }
                    else if (warning_sign(approx_sign_x, approx_sign_y) == true)
                    {
                        cv::drawContours(img, contours, x, cv::Scalar(125, 255, 0), 2);
                    }
                }
            }
        }
    }

    cv::imshow("Triangles", img);
    cv::waitKey(0);
}

} // namespace

void red_contours(cv::Mat bgr_image)
{
    cv::medianBlur(bgr_image, bgr_image, 3);

    cv::Mat hsv_image;
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

    cv::Mat lower_red_hue_range;
    cv::Mat upper_red_hue_range;
    cv::inRange(hsv_image, cv::Scalar(0, 70, 50), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(hsv_image, cv::Scalar(170, 70, 50), cv::Scalar(180, 255, 255), upper_red_hue_range);

    cv::Mat red_hue_image;
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    findContours(red_hue_image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    for (std::size_t x = 0; x < contours.size(); x++)
    {
        cv::drawContours(red_hue_image, contours, x, cv::Scalar(255, 255, 255), 3);
    }

    findContours(red_hue_image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));


    find_red_triangles(bgr_image, contours);
}

int main(int argc, char **argv) {
    std::chrono::time_point<std::chrono::system_clock> start, end;

    cv::Mat bgr_image = cv::imread(argv[1]);

    start = std::chrono::system_clock::now();

    red_contours(bgr_image);

    end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time for frame: " << diff.count() << std::endl;
    std::cout << "FPS: " << (int)(1.0 / diff.count()) << std::endl;

    return 0;
}
