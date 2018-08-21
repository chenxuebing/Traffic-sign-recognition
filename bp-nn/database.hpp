#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#define IMG_SIZE 50

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
    std::string                 en_name;
    std::string                 ru_name;
    std::string                 ua_name;
} Sign_params_t;

typedef std::vector<Sign_params_t> sign_array_t;

const sign_array_t sign_array = {
    {
        0,
        red_c,
        "Give priority to vehicles from opposite direction ",
        "Преимущество встречного движения",
        "Перевага зустрічного руху"
    },
    {
        1,
        red_c,
        "No stopping",
        "Остановка запрещена",
        "Зупинку заборонено"
    },
    {
        2,
        red_c,
        "Give way",
        "Уступить дорогу",
        "Дати дорогу"
    },
    {
        3,
        red_c,
        "No left turn",
        "Поворот в левую сторону запрещен",
        "Поворот ліворуч заборонено"
    },
    {
        4,
        red_c,
        "Stop and give way ",
        "Проезд без остановки запрещен",
        "Проїзд без зупинки заборонено"
    },
    {
        5,
        red_c,
        "Give way",
        "Опасный поворот направо",
        "Bend to right"
    },
    {
        6,
        red_c,
        "Level crossing without barrier",
        "Железнодорожный переезд без шлагбаума",
        "Залізничний переїзд без шлагбаума"
    },
    {
        7,
        red_c,
        "No entry",
        "Въезд запрещен",
        "В'їзд заборонено"
    },
};

std::vector<std::pair<int, cv::Mat>> get_images(contours_t color, std::string path);
void normalize_image(cv::Mat& image);
float* image_to_array(cv::Mat image);
