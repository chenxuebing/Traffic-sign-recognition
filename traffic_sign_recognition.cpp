#include <exception>
// #include <QDebug>

#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/image_transforms.h>
#include <dlib/cmd_line_parser.h>
#include <dlib/opencv.h>

#include "traffic_sign_recognition.h"
#include "database.hpp"

cv::Mat tsr(cv::Mat frameGray, int upsample_amount)
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

        typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;
        std::vector<dlib::object_detector<image_scanner_type>> detectors;

        for (auto i = signs.begin(); i != signs.end(); i++)
        {
            dlib::object_detector<image_scanner_type> detector;

            dlib::deserialize(i->detector) >> detector;
            detectors.push_back(detector);
        }

        std::vector<dlib::rect_detection> rects;

        evaluate_detectors(detectors, dlibImageGray, rects);

        for (auto i = rects.begin(); i != rects.end(); i++)
        {
            cv::Rect rect = cv::Rect(cv::Point2i(i->rect.left(), i->rect.top()), cv::Point2i(i->rect.right(), i->rect.bottom()));
            rectangle(filterImage, rect,  cv::Scalar(0, 255, 0), 2);
            cv::putText(filterImage, signs[i->weight_index].en_name, cv::Point(i->rect.right(), (i->rect.bottom() + i->rect.top()) / 2), 
                cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0));
        }

        return filterImage;
    }
    catch (std::exception& e)
    {
         // qDebug() << "tsr: " << e.what();
         throw ;
    }
}
