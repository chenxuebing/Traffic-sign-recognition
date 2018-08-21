// #include <dirent.h>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "database.hpp"

// std::vector<std::pair<int, cv::Mat>> get_images(contours_t color)
// {
//     std::vector<std::pair<int, cv::Mat>> images;
//     std::vector<std::string> image_names;
//     DIR *dir;
//     struct dirent *ent;

//     if ((dir = opendir("db")) != NULL)
//     {
//         while ((ent = readdir(dir)) != NULL)
//         {
//             image_names.push_back(ent->d_name);
//         }

//         closedir(dir);
//     }

//     for (auto i = sign_array.begin(); i != sign_array.end(); i++)
//     {
//         if (i->contour == color)
//         {
//             for (int n = 0; n < 10; n ++)
//             {
//                 std::string part_name = std::to_string(i->id) + "_0" + std::to_string(n);

//                 auto it = std::find_if(image_names.begin(), image_names.end(), 
//                     [&part_name](std::string const& image_name)
//                     { 
//                         return image_name.find(part_name) != std::string::npos;
//                     }
//                 );

//                 if (it != image_names.end())
//                 {
//                     cv::Mat cv_image = cv::imread("./db/" + *it, CV_LOAD_IMAGE_GRAYSCALE);
//                     cv::Mat cv_resize_image;

//                     cv::resize(cv_image, cv_resize_image, cv::Size(IMG_SIZE, IMG_SIZE));
//                     images.push_back(std::pair<int, cv::Mat>(i->id, cv_resize_image));
//                 }
//             }
//         }
//     }

//     return images;  
// }

std::vector<std::pair<int, cv::Mat>> get_images(contours_t color, std::string path)
{
    std::vector<std::pair<int, cv::Mat>> images;

    for (auto i = sign_array.begin(); i != sign_array.end(); i++)
    {
        if (i->contour == color)
        {
            std::string name = path + std::to_string(i->id) + "_icon.png";

            cv::Mat cv_image = cv::imread(name, CV_LOAD_IMAGE_GRAYSCALE);
            cv::Mat cv_resize_image;

            cv::resize(cv_image, cv_resize_image, cv::Size(IMG_SIZE, IMG_SIZE));
            normalize_image(cv_resize_image);
            images.push_back(std::pair<int, cv::Mat>(i->id, cv_resize_image));
        }
    }

    return images;  
}

void normalize_image(cv::Mat& image)
{
    int max = 0;
    int min = 255;

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            if (max < static_cast<int>(image.at<uchar>(j,i)))
            {
                max = static_cast<int>(image.at<uchar>(j,i));
            }
            if (min > static_cast<int>(image.at<uchar>(j,i)))
            {
                min = static_cast<int>(image.at<uchar>(j,i));
            }
        }
    }

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            image.at<uchar>(j,i) = 255 * (static_cast<int>(image.at<uchar>(j,i)) - min) / (max - min);
        }
    }
}

float* image_to_array(cv::Mat image)
{
    float *output = new float[image.rows * image.cols];
    std::size_t n = 0;

    normalize_image(image);

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            output[n] = 127.0 / (static_cast<int>(image.at<uchar>(j,i)));
            n++;
        }
    }

    return output;
}
