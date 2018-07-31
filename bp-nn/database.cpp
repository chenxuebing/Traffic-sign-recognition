#include <dirent.h>
#include <algorithm>
#include "database.hpp"

std::vector<std::pair<int, cv::Mat>> get_images(contours_t color)
{
    std::vector<std::pair<int, cv::Mat>> images;
    std::vector<std::string> image_names;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("db")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            image_names.push_back(ent->d_name);
        }

        closedir(dir);
    }

    for (auto i = sign_array.begin(); i != sign_array.end(); i++)
    {
        if (i->contour == color)
        {
            for (int n = 0; n < 10; n ++)
            {
                std::string part_name = std::to_string(i->id) + "_0" + std::to_string(n);

                auto it = std::find_if(image_names.begin(), image_names.end(), 
                    [&part_name](std::string const& image_name)
                    { 
                        return image_name.find(part_name) != std::string::npos;
                    }
                );

                if (it != image_names.end())
                {
                    cv::Mat cv_image = cv::imread("./db/" + *it, CV_LOAD_IMAGE_GRAYSCALE);
                    cv::Mat cv_resize_image;

                    cv::resize(cv_image, cv_resize_image, cv::Size(50, 50));
                    images.push_back(std::pair<int, cv::Mat>(i->id, cv_resize_image));
                }
            }
        }
    }

    images.insert( images.end(), images.begin(), images.end() );
    images.insert( images.end(), images.begin(), images.end() );

    return images;  
}

double* image_to_array(cv::Mat image)
{
    double *output = new double[image.rows * image.cols];

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            output[i] = 127.0 / (double)(image.at<uchar>(j,i));
        }
    }

    return output;
}
