#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

#include "../bp-nn/database.hpp"

namespace {

int compare_images(cv::Mat img_in, std::vector<std::pair<int, cv::Mat>> img_v)
{
    cv::Mat resize_image;
    cv::resize(img_in, resize_image, cv::Size(IMG_SIZE, IMG_SIZE));

    cv::Mat img_1;
    cv::cvtColor(resize_image, img_1, CV_BGR2GRAY);
    normalize_image(img_1);


    for (auto i = img_v.begin(); i != img_v.end(); i++)
    {
        cv::Mat img_2(i->second);

        //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors

        int minHessian = 400;

        cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create();
        detector->setHessianThreshold(minHessian);
        std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
        cv::Mat descriptors_1, descriptors_2;

        // img_1 = Closing[Binarize[GeodesicOpening[img_1, DiskMatrix[5]], Method->Entropy], 1];

        detector->detectAndCompute(img_1, cv::Mat(), keypoints_1, descriptors_1);
        detector->detectAndCompute(img_2, cv::Mat(), keypoints_2, descriptors_2);

        //-- Step 2: Matching descriptor vectors using FLANN matcher

        cv::FlannBasedMatcher matcher;
        // BFMatcher matcher;

        std::vector<cv::DMatch> matches;

        if (descriptors_1.empty() || descriptors_2.empty())
            continue;

        matcher.match(descriptors_1, descriptors_2, matches);

        double max_dist = 0; double min_dist = 100;

        //-- Quick calculation of max and min distances between keypoints

        for (int i = 0; i < descriptors_1.rows; i++)
        {
            double dist = matches[i].distance;

            if (dist < min_dist)
            min_dist = dist;

            if (dist > max_dist)
            max_dist = dist;
        }

        printf("-- Max dist : %f \n", max_dist );
        printf("-- Min dist : %f \n", min_dist );

        //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
        //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very small)
        //-- PS.- radiusMatch can also be used here.

        std::vector<cv::DMatch> good_matches;
        for (int i = 0; i < descriptors_1.rows; i++)
        {
            if (matches[i].distance <= cv::max(2 * min_dist, 0.02))
            {
                good_matches.push_back(matches[i]);
            }
        }

        //-- Draw only "good" matches

        cv::Mat img_matches;

        cv::drawMatches( img_1, keypoints_1, img_2, keypoints_2,
        good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );


        // for (int i = 0; i < (int)good_matches.size(); i++)
        // {
        //     printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx );
        // }

        // // -- Localize the object
        // std::vector<cv::Point2f> obj;
        // std::vector<cv::Point2f> scene;

        // for (int i = 0; i < good_matches.size(); i++)
        // {
        //     //-- Get the keypoints from the good matches
        //     obj.push_back(keypoints_1[ good_matches[i].queryIdx ].pt);
        //     scene.push_back(keypoints_2[ good_matches[i].trainIdx ].pt);
        // }

        // cv::Mat H = cv::findHomography(obj, scene, CV_RANSAC);

        // //-- Get the corners from the image_1 ( the object to be "detected" )
        // std::vector<cv::Point2f> obj_corners(4);
        // obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint(img_1.cols, 0);
        // obj_corners[2] = cvPoint(img_1.cols, img_1.rows); obj_corners[3] = cvPoint(0, img_1.rows);
        // std::vector<cv::Point2f> scene_corners(4);

        // cv::perspectiveTransform(obj_corners, scene_corners, H);

        // //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        // cv::line(img_matches, scene_corners[0] + cv::Point2f(img_1.cols, 0), scene_corners[1] + cv::Point2f(img_1.cols, 0), cv::Scalar(0, 255, 0), 2);
        // cv::line(img_matches, scene_corners[1] + cv::Point2f(img_1.cols, 0), scene_corners[2] + cv::Point2f(img_1.cols, 0), cv::Scalar(0, 255, 0), 2);
        // cv::line(img_matches, scene_corners[2] + cv::Point2f(img_1.cols, 0), scene_corners[3] + cv::Point2f(img_1.cols, 0), cv::Scalar(0, 255, 0), 2);
        // cv::line(img_matches, scene_corners[3] + cv::Point2f(img_1.cols, 0), scene_corners[0] + cv::Point2f(img_1.cols, 0), cv::Scalar(0, 255, 0), 2);

        // //-- Show detected matches

        cv::imshow("Good Matches", img_matches);

        cv::waitKey(0);

        std::cout << "end loop" << std::endl;    
    }

    return 0;
}

} // namespace

void red_contours(cv::Mat bgr_image)
{
    std::vector<std::pair<int, cv::Mat>> red_images = get_images(red_c, "../bp-nn/db/");

    cv::medianBlur(bgr_image, bgr_image, 3);

    cv::Mat hsv_image;
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

    cv::Mat lower_red_hue_range;
    cv::Mat upper_red_hue_range;
    cv::inRange(hsv_image, cv::Scalar(0, 70, 50), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(hsv_image, cv::Scalar(170, 70, 50), cv::Scalar(180, 255, 255), upper_red_hue_range);

    cv::Mat red_hue_image;
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    findContours(red_hue_image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    for (std::size_t x = 0; x < contours.size(); x++)
    {
        cv::drawContours(red_hue_image, contours, x, cv::Scalar(255, 255, 255), 3);
    }

    findContours(red_hue_image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    for (std::size_t x = 0; x < contours.size(); x++)
    {
        cv::Mat mask(bgr_image.rows, bgr_image.cols, CV_8UC3);
        cv::drawContours(mask, contours, x, cv::Scalar(255, 255, 255), CV_FILLED);

        cv::Mat crop(bgr_image.rows, bgr_image.cols, CV_8UC3);
        crop.setTo(cv::Scalar(0,0,0));
        bgr_image.copyTo(crop, mask);

        compare_images(crop(cv::boundingRect(contours[x])), red_images);

        // cv::drawContours(bgr_image, contours, x, cv::Scalar(125, 255, 0), 2);
    }

    cv::imshow("Triangles", bgr_image);
    cv::waitKey(0);
}

int main(int argc, char **argv) {
    std::chrono::time_point<std::chrono::system_clock> start, end;

    cv::Mat bgr_image = cv::imread(argv[1]);

    start = std::chrono::system_clock::now();

    red_contours(bgr_image);

    end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time for frame: " << diff.count() << std::endl;
    std::cout << "FPS: " << (int)(1.0 / diff.count()) << std::endl;

    return 0;
}
