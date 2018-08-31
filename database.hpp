#pragma once

#include <iostream>
#include <vector>
#include <QObject>

typedef enum roadDirections_t
{
    straight_road = 0,
    left_road = 1,
    right_road = 2,
    road_not_found = 4,
} roadDirections_t;

typedef struct Sign_params_t
{
    int                     id;
    QString                 detector;
    std::vector<QString>    name;
} Sign_params_t;

const std::vector<std::vector<QString>> roadDirections = {
    {"STRAIGHT", "ПРЯМО", "ПРЯМО"},
    {"LEFT", "ЛЕВЕЕ", "ЛІВІШЕ"},
    {"RIGHT", "ПРАВЕЕ", "ПРАВІШЕ"}
};

const std::vector<Sign_params_t> signs = {
    {
        0,
        ":transito-cv/svm_detectors/0.svm",
        {
            "Give way",
            "Уступить дорогу",
            "Дати дорогу"
        }
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
