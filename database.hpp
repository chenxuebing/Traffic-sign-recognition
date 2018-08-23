#pragma once

#include <iostream>
#include <vector>

typedef enum contours_t
{
    red_c = 101,
    blue_c = 102,
    yellow_c = 103
} contours_t;

typedef struct Sign_params_t
{
    int                         id;
    contours_t                  contour;
    std::string                 detector;
    std::string                 en_name;
    std::string                 ru_name;
    std::string                 ua_name;
} Sign_params_t;

typedef std::vector<Sign_params_t> sign_array_t;

const sign_array_t signs = {
    {
        0,
        red_c,
        "/home/amyrhorod/android_template/transito-cv/svm_detectors/0.svm",
        "Give way",
        "Уступить дорогу",
        "Дати дорогу"
    }//,
//    {
//        1,
//        red_c,
//        "Give priority to vehicles from opposite direction ",
//        "Преимущество встречного движения",
//        "Перевага зустрічного руху"
//    },
//    {
//        2,
//        red_c,
//        "No stopping",
//        "Остановка запрещена",
//        "Зупинку заборонено"
//    },
//    {
//        3,
//        red_c,
//        "No left turn",
//        "Поворот в левую сторону запрещен",
//        "Поворот ліворуч заборонено"
//    },
//    {
//        4,
//        red_c,
//        "Stop and give way ",
//        "Проезд без остановки запрещен",
//        "Проїзд без зупинки заборонено"
//    },
//    {
//        5,
//        red_c,
//        "Give way",
//        "Опасный поворот направо",
//        "Bend to right"
//    },
//    {
//        6,
//        red_c,
//        "Level crossing without barrier",
//        "Железнодорожный переезд без шлагбаума",
//        "Залізничний переїзд без шлагбаума"
//    },
//    {
//        7,
//        red_c,
//        "No entry",
//        "Въезд запрещен",
//        "В'їзд заборонено"
//    },
};
