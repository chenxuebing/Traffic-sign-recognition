#ifndef ROAD_VIDEO_FILTER_H
#define ROAD_VIDEO_FILTER_H

#include <QObject>
#include <QAbstractVideoFilter>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <unordered_map>
#include <QDebug>
#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/image_transforms.h>
#include <dlib/cmd_line_parser.h>
#include <dlib/opencv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <exception>

#include "database.hpp"

typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;

class RoadVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    RoadVideoFilterRunnable(void* rvf);
    QVideoFrame run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);

private:
    void    _detect();
    void    _tsr(cv::Mat frameGray, cv::Mat& filter);
    void    _tld(cv::Mat frameGray, cv::Mat& filter);

    cv::Mat _filter;
    cv::Mat _frameRGB;

    std::mutex              _filterMutex;
    std::mutex              _frameRGBMutex;
    std::mutex              _detectedSignsMutex;
    std::condition_variable _newFrame;

    std::vector<std::thread>                                _threads;
    std::vector<dlib::object_detector<image_scanner_type>>  _detectors;

    std::unordered_map<int, std::chrono::time_point<std::chrono::system_clock>> _detectedSigns;

    void* _rvf;
};

class RoadVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE
    {
        return new RoadVideoFilterRunnable(this);
    }

    std::size_t lenguageID = 0;

signals:
    void newSign(int id, QString name);
    void roadStatus(QString status);
};

#endif // ROAD_VIDEO_FILTER_H
