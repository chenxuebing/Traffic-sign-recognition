#include <QDebug>

#include "road_video_filter.h"

RoadVideoFilterRunnable::RoadVideoFilterRunnable()
{
    unsigned int nthreads = std::thread::hardware_concurrency();

    for (unsigned int i = 0; i < nthreads; i++)
    {
        _threads.push_back(std::thread(/*...*/));
    }
}

QVideoFrame RoadVideoFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags)
{
    if (!input->isValid() || !input->map(QAbstractVideoBuffer::ReadOnly))
    {
        return *input;
    }

    try
    {
        cv::Mat frameYUV = cv::Mat(input->height() + input->height()/2, input->width(), CV_8UC1, static_cast<void*>(input->bits()));

        cv::Mat frameRGB_res;
//        cv::Mat frameGray;

        {
            std::unique_lock<std::mutex> lock(_frameRGBMutex);

            cv::cvtColor(frameYUV, _frameRGB, CV_YUV2BGRA_I420);
            _newFrame.notify_all();
        }

//        cv::cvtColor(frameRGB, frameGray, CV_BGR2GRAY);

//        cv::Mat filter = tsr(frameGray);

        {
            std::unique_lock<std::mutex> lock(_filterMutex);

            cv::addWeighted(_filter, 1.0, _frameRGB, 1.0, 0, frameRGB_res);
        }

        QImage q_image_frame = QImage(static_cast<uchar*>(frameRGB_res.data), frameRGB_res.cols, frameRGB_res.rows, QImage::Format_RGB32);

        return QVideoFrame(q_image_frame);
//    }
//    catch (std::exception& e)
//    {
//         qDebug() << "RoadVideoFilterRunnable::run: " << e.what();
//    }

    return *input;
}
