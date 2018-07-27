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
    std::string                 en_name;
    std::string                 ru_name;
    std::string                 ua_name;
} Sign_params_t;

typedef std::vector<Sign_params_t> sign_array_t;

const sign_array_t sign_array = {
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
    }
};

