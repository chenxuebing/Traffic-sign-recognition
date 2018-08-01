#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#define NUM_IN      50 * 50
#define NUM_HID     30
#define NUM_OUT     5
#define MAX_EPOCHS  200

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
        "Movement without stopping is forbidden",
        "Движение без остановки запрещено",
        "Рух без зупинки заборонено"
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
        "No entry",
        "Въезд запрещен",
        "В'їзд заборонено"
    }
};


std::vector<std::pair<int, cv::Mat>>    get_images(contours_t color);
float*                                  image_to_array(cv::Mat image);
cv::Mat                                 preapere_image(cv::Mat image);
Sign_params_t                           param_by_id(int id);
