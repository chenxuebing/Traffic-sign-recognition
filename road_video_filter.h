#ifndef ROAD_VIDEO_FILTER_H
#define ROAD_VIDEO_FILTER_H

#include <QObject>
#include <QAbstractVideoFilter>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <unordered_map>

#include "traffic_sign_recognition.h"

class RoadVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    RoadVideoFilterRunnable(void* rvf);
    QVideoFrame run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);

private:
    void    _detect();

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
