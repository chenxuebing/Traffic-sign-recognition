#include <QDebug>

#include "road_video_filter.h"
#include "traffic_sign_recognition.h"

QVideoFrame RoadVideoFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags)
{
    if (!input->isValid() || !input->map(QAbstractVideoBuffer::ReadOnly))
    {
        return *input;
    }

    try
    {
        cv::Mat frameYUV = cv::Mat(input->height() + input->height()/2, input->width(), CV_8UC1, static_cast<void*>(input->bits()));

        cv::Mat frameRGB;
        cv::Mat frameRGB_res(frameRGB);
        cv::Mat frameGray;

        cv::cvtColor(frameYUV, frameRGB, CV_YUV2BGRA_I420);
        cv::cvtColor(frameRGB, frameGray, CV_BGR2GRAY);

//        tsr(frameGray, frameRGB_res);

        QImage q_image_frame = QImage(static_cast<uchar*>(frameRGB_res.data), frameRGB_res.cols, frameRGB_res.rows, QImage::Format_RGB32);

        return QVideoFrame(q_image_frame);
    }
    catch (std::exception& e)
    {
         qDebug() << "RoadVideoFilterRunnable::run: " << e.what();
    }

    return *input;
}
