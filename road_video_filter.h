#ifndef ROAD_VIDEO_FILTER_H
#define ROAD_VIDEO_FILTER_H

#include <QObject>
#include <QAbstractVideoFilter>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "traffic_sign_recognition.h"

class RoadVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    RoadVideoFilterRunnable();
    QVideoFrame run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);

private:
    cv::Mat _filter;
    cv::Mat _frameRGB;

    std::mutex                  _filterMutex;
    std::mutex                  _frameRGBMutex;
    std::condition_variable     _newFrame;

    std::vector<std::thread>    _threads;
};

class RoadVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE
    {
        return new RoadVideoFilterRunnable;
    }
};

#endif // ROAD_VIDEO_FILTER_H
