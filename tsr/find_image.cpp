/*
g++ main.cpp `pkg-config --cflags --libs opencv` -std=c++11
 */

#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main( int argc, char** argv )
{
    if( argc != 3 )
    {
        std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl;
        return -1;
    }

    Mat img_1 = (imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE));
    Mat img_2 = (imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE));

    if( !img_1.data || !img_2.data )
    {
        std::cout<< " --(!) Error reading images " << std::endl; return -1;
    }

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors

    int minHessian = 400;

    Ptr<SURF> detector = SURF::create();
    detector->setHessianThreshold(minHessian);
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;

    // img_1 = Closing[Binarize[GeodesicOpening[img_1, DiskMatrix[5]], Method->Entropy], 1];

    detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
    detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );

    //-- Step 2: Matching descriptor vectors using FLANN matcher

    FlannBasedMatcher matcher;
    // BFMatcher matcher;

    std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );
    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints

    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;

        if( dist < min_dist )
        min_dist = dist;

        if( dist > max_dist )
        max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );

    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very small)
    //-- PS.- radiusMatch can also be used here.

    std::vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        if( matches[i].distance <= max(2 * min_dist, 0.02) )
        {
            good_matches.push_back( matches[i]);
        }
    }

    //-- Draw only "good" matches

    Mat img_matches;

    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
    good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
    vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );


    for( int i = 0; i < (int)good_matches.size(); i++ )
    {
        printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx );
    }

    // -- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( int i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_1[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_2[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, CV_RANSAC );

    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_1.cols, 0 );
    obj_corners[2] = cvPoint( img_1.cols, img_1.rows ); obj_corners[3] = cvPoint( 0, img_1.rows );
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform(obj_corners, scene_corners, H);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    line( img_matches, scene_corners[0] + Point2f( img_1.cols, 0), scene_corners[1] + Point2f( img_1.cols, 0), Scalar(0, 255, 0), 2);
    line( img_matches, scene_corners[1] + Point2f( img_1.cols, 0), scene_corners[2] + Point2f( img_1.cols, 0), Scalar( 0, 255, 0), 2);
    line( img_matches, scene_corners[2] + Point2f( img_1.cols, 0), scene_corners[3] + Point2f( img_1.cols, 0), Scalar( 0, 255, 0), 2);
    line( img_matches, scene_corners[3] + Point2f( img_1.cols, 0), scene_corners[0] + Point2f( img_1.cols, 0), Scalar( 0, 255, 0), 2);

    //-- Show detected matches

    imshow( "Good Matches", img_matches );

    waitKey(0);
    return 0;
}
