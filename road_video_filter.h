#ifndef ROAD_VIDEO_FILTER_H
#define ROAD_VIDEO_FILTER_H

#include <QObject>
#include <QAbstractVideoFilter>

class RoadVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    QVideoFrame run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);
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
