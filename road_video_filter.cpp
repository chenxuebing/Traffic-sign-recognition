#include "road_video_filter.h"

RoadVideoFilterRunnable::RoadVideoFilterRunnable()
{
    unsigned int nthreads = std::thread::hardware_concurrency();
    std::chrono::milliseconds wait(1000 / nthreads);

    for (auto i = signs.begin(); i != signs.end(); i++)
    {
        dlib::object_detector<image_scanner_type> detector;

        dlib::deserialize(i->detector) >> detector;
        _detectors.push_back(detector);
    }

    for (unsigned int i = 0; i < nthreads; i++)
    {
        _threads.push_back(std::thread(&RoadVideoFilterRunnable::_detect, this));
        std::this_thread::sleep_for(wait);
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
        cv::Mat frameRGB_res;
        cv::Mat frameYUV = cv::Mat(input->height() + input->height()/2, input->width(), CV_8UC1, static_cast<void*>(input->bits()));

        {
            std::unique_lock<std::mutex> lock(_frameRGBMutex);

            cv::cvtColor(frameYUV, _frameRGB, cv::COLOR_YUV420p2RGB);

            _newFrame.notify_one();
            frameRGB_res = _frameRGB;
        }

        if (_filter.rows != 0)
        {
            std::unique_lock<std::mutex> lock(_filterMutex);

            cv::addWeighted(_filter, 1.0, _frameRGB, 1.0, 0, frameRGB_res);
        }

        cv::cvtColor(frameRGB_res, frameYUV, CV_RGB2YUV_YV12);
        QImage q_image_frame = QImage(frameYUV.data, frameYUV.cols, frameYUV.rows, QVideoFrame::imageFormatFromPixelFormat(input->pixelFormat()));

        return QVideoFrame(q_image_frame);
    }
    catch (std::exception& e)
    {
         qDebug() << "RoadVideoFilterRunnable::run: " << e.what();
    }

    return *input;
}

void RoadVideoFilterRunnable::_detect()
{
    while (true)
    {
        try
        {
            while (true)
            {
                qDebug() << "start";

                cv::Mat frameGray;

                {
                    std::unique_lock<std::mutex> lock(_frameRGBMutex);
                    _newFrame.wait(lock);
                    cv::cvtColor(_frameRGB, frameGray, CV_RGB2GRAY);
                }

                cv::Mat newFilter = tsr(frameGray, _detectors);

                {
                    std::unique_lock<std::mutex> lock(_filterMutex);
                    _filter = newFilter;
                }

                qDebug() << "end";
            }
        }
        catch (std::exception& e)
        {
             qDebug() << "RoadVideoFilterRunnable::_detect: " << e.what();
        }
    }
}
