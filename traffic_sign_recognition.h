#ifndef TRAFFIC_SIGN_RECOGNITION_H
#define TRAFFIC_SIGN_RECOGNITION_H

#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/image_transforms.h>
#include <dlib/cmd_line_parser.h>

#include <iostream>

void tsr(dlib::array2d<unsigned char> &image, int upsample_amount);
void test();

#endif // TRAFFIC_SIGN_RECOGNITION_H
