#include <exception>
#include <cmath>

#include "traffic_sign_recognition.h"

cv::Mat tsr(cv::Mat frameGray, std::vector<dlib::object_detector<image_scanner_type>> detectors, int upsample_amount)
{
    try
    {
        cv::Mat filterImage(frameGray.rows, frameGray.cols, CV_8UC3, cv::Scalar(0, 0, 0));
        dlib::array2d<unsigned char> dlibImageGray;

        dlib::assign_image(dlibImageGray, dlib::cv_image<unsigned char>(frameGray));

        for (int i = 0; i < upsample_amount; ++i)
        {
            pyramid_up(dlibImageGray);
        }

        std::vector<dlib::rect_detection> rects;

        evaluate_detectors(detectors, dlibImageGray, rects);

        for (auto i = rects.begin(); i != rects.end(); i++)
        {
            cv::Rect rect = cv::Rect(cv::Point2i(i->rect.left(), i->rect.top()), cv::Point2i(i->rect.right(), i->rect.bottom()));
            rectangle(filterImage, rect,  cv::Scalar(0, 255, 0), 2);
            cv::putText(filterImage, signs[i->weight_index].en_name, cv::Point(i->rect.right(), (i->rect.bottom() + i->rect.top()) / 2), 
                cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0), 2);

            // qDebug() << signs[i->weight_index].en_name.c_str();
        }

        return filterImage;
    }
    catch (std::exception& e)
    {
         // qDebug() << "tsr: " << e.what();
         throw ;
    }
}

void tld(cv::Mat frameGray, cv::Mat& filter)
{
    try
    {
        int height = frameGray.rows;
        int width = frameGray.cols;

        cv::Mat croppedImage(frameGray.rows, frameGray.cols, CV_8UC1, cv::Scalar(0));;

        cv::Canny(frameGray, croppedImage, 100, 200);

        std::vector<cv::Point> mask_1, mask_2;

        mask_1.push_back(cv::Point(0, 0));
        mask_1.push_back(cv::Point(0, height));
        mask_1.push_back(cv::Point(width / 2, height / 2));
        mask_1.push_back(cv::Point(width, 0));
        mask_2.push_back(cv::Point(width, 0));
        mask_2.push_back(cv::Point(width / 2, height / 2));
        mask_2.push_back(cv::Point(width, height));

        cv::fillConvexPoly(croppedImage, mask_1, cv::Scalar(0));
        cv::fillConvexPoly(croppedImage, mask_2, cv::Scalar(0));

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(croppedImage, lines, 6, CV_PI / 60.0, 160, 40, 25);

        std::vector<cv::Vec2f> left_line_x, left_line_y, right_line_x, right_line_y;
        for (std::size_t i = 0; i < lines.size(); i++)
        {
            float slope = (lines[i][3] - lines[i][1]) / (lines[i][2] - lines[i][0]);

            if (fabs(slope) < 0.5)
            {
                continue ;
            }
            if (slope <= 0)
            {
                left_line_x.push_back(cv::Vec2f(lines[i][0], lines[i][2]));
                left_line_y.push_back(cv::Vec2f(lines[i][1], lines[i][3]));
            }
            else
            {
                right_line_x.push_back(cv::Vec2f(lines[i][0], lines[i][2]));
                right_line_y.push_back(cv::Vec2f(lines[i][1], lines[i][3]));
            }
        }

        int min_y = height * (3 / 5);
        int max_y = height;
    
        exit(0);
    }
    catch (std::exception& e)
    {
        std::cout << "tld: " << e.what() << std::endl;
         // qDebug() << "tld: " << e.what();
        throw ;
    }
}
