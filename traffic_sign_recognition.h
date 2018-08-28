#ifndef TRAFFIC_SIGN_RECOGNITION_H
#define TRAFFIC_SIGN_RECOGNITION_H

#include <QDebug>
#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/image_transforms.h>
#include <dlib/cmd_line_parser.h>
#include <dlib/opencv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "database.hpp"

typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;

cv::Mat tsr(cv::Mat frameGray, std::vector<dlib::object_detector<image_scanner_type>> detectors, int upsample_amount = 0);

#endif // TRAFFIC_SIGN_RECOGNITION_H
