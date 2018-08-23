#include <exception>
#include <QDebug>

#include "traffic_sign_recognition.h"
#include "database.hpp"

void tsr(dlib::array2d<unsigned char> &image, int upsample_amount = 0)
{
    try
    {
        for (int i = 0; i < upsample_amount; ++i)
        {
            pyramid_up(image);
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

        evaluate_detectors(detectors, image, rects);

        for (unsigned long j = 0; j < rects.size(); ++j) {
            dlib::draw_rectangle(image, rects[j].rect, dlib::rgb_pixel(0, 255, 0));
//            win.add_overlay(rects[j].rect, dlib::rgb_pixel(255,0,0), signs[rects[j].weight_index].en_name);
        }
    }
    catch (std::exception& e) {
         qDebug() << "tsr: " << e.what();
    }
}

void test()
{
    try
    {
        dlib::array2d<unsigned char> image;

        load_image(image, "/home/amyrhorod/Downloads/Screen.png");

//        tsr(image, 1);

        dlib::image_window win;

        win.clear_overlay();
        win.set_image(image);

        while(true);
    }

    catch (std::exception& e)
    {
        qDebug() << "tsr: " << e.what();
    }
}
