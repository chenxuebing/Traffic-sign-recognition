#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void detect_sign(cv::Mat img_rgb);
void find_all_triangles(cv::Mat&, const std::vector<std::vector<cv::Point>>&);

int thresh = 100;
int max_thresh = 255;

int main(int argc, char** argv)
{
    cv::Mat img_rgb  = cv::imread(argv[1]);

    detect_sign(img_rgb);

    cv::waitKey(0);
}

void detect_sign(cv::Mat img_rgb)
{
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat img_gray;
    cv::cvtColor(img_rgb,img_gray, CV_RGB2GRAY);

    cv::Mat canny_output;
    cv::Canny( img_gray, canny_output, thresh, thresh*2, 3 );

    cv::i
    mshow("Canny",canny_output);
    
    cv::findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    cv::Mat drawing;
    drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );

    find_all_triangles(img_rgb, contours);
    cv::imshow("Triangles",img_rgb);
}

void find_all_triangles(cv::Mat& img, const std::vector<std::vector<cv::Point>>& contours)
{
    std::vector<cv::Point> approxTriangle;

    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::approxPolyDP(contours[i], approxTriangle, cv::arcLength(cv::Mat(contours[i]), true)*0.05, true);

        if (approxTriangle.size() == 3)
        {
            cv::drawContours(img, contours, i, cv::Scalar(0, 255, 255), CV_FILLED); // fill GREEN
            
            std::vector<cv::Point>::iterator vertex;
            
            for (vertex = approxTriangle.begin(); vertex != approxTriangle.end(); ++vertex)
            {
                cv::circle(img, *vertex, 3, cv::Scalar(0, 0, 255), 1);
            }
        }
    }
}