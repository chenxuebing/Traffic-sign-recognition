#include "tsr.hpp"

TSR::TSR() : nn(NUM_IN, NUM_HID, NUM_OUT)
{
    nn.loadWeights("../bp-nn/weights.dat");
}

std::pair<cv::Mat, std::vector<Sign_params_t>> TSR::sign_from_img(cv::Mat bgr_image, contours_t color)
{

    cv::Mat orig_image = bgr_image.clone();

    cv::medianBlur(bgr_image, bgr_image, 3);

    cv::Mat hsv_image;
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

    cv::Mat lower_color_hue_range;
    cv::Mat upper_color_hue_range;

    if (color == red_c)
    {
        cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_color_hue_range);
        cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_color_hue_range);
    }
    else
    {
        // TODO
    }

    cv::Mat color_hue_image;
    cv::addWeighted(lower_color_hue_range, 1.0, upper_color_hue_range, 1.0, 0.0, color_hue_image);

    cv::GaussianBlur(color_hue_image, color_hue_image, cv::Size(9, 9), 2, 2);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<Sign_params_t> sign_params;

    findContours(color_hue_image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    for (std::size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect bounding_rect = boundingRect(contours[i]);
        cv::Mat sub_image(bgr_image(bounding_rect));

        if (sub_image.rows > bgr_image.rows / 16)
        {
            cv::Mat gray_image;
            cv::cvtColor(sub_image, gray_image, CV_BGR2GRAY);
            
            cv::Mat cv_resize_image;
            cv::resize(gray_image, cv_resize_image, cv::Size(50, 50));

            float* array = image_to_array(cv_resize_image);

            std::pair<float, int> result_sign = nn.classify(array);

            delete [] array;

            std::cout << "percent: " << result_sign.first << " id: " << result_sign.second << std::endl;

            if (result_sign.first > 0.5)
            {
                Sign_params_t sign = param_by_id(result_sign.second);

                sign_params.push_back(sign);

                rectangle(bgr_image, bounding_rect, cv::Scalar(0, 255, 0), 2, 8, 0);
            
                std::cout << ">>> Found: " << sign.ru_name << std::endl;
            }
        }
    }

    return std::pair<cv::Mat, std::vector<Sign_params_t>>(bgr_image, sign_params);
}

