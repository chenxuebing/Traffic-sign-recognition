#include <cmath>
#include <QFile>
#include <QDir>
#include <chrono>
#include <QStandardPaths>
#include <QFileInfo>
#include <QVariant>

#include "road_video_filter.h"

namespace {

/*
 * Get width by normilized vector
*/
int WbyNormVec(float h, float x, float y, float v0, float v1)
{
    float d = (y - h) / v1;
    float w = x - (d * v0);

    return static_cast<int>(w);
}

} // namespace

RoadVideoFilterRunnable::RoadVideoFilterRunnable(void* rvf) : _rvf(rvf)
{
    /*
     * Prepare neural networks samples
    */
    {
        QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tsr";

        if (!QDir(path).exists())
        {
            QDir().mkdir(path);
        }

        for (auto i = signs.begin(); i != signs.end(); i++)
        {
            QString fileName = QVariant(i->id).toString() + ".svm";

            /*
             * Unpack neural networks samples
            */

            if (!QFileInfo(path + "/" + fileName).exists())
            {
                QFile fileIn(i->detector);

                if (!fileIn.open(QIODevice::ReadOnly))
                {
                    qDebug() << fileIn.errorString() << ": " << i->detector;
                    continue ;
                }

                QFile fileOut(path + "/" + fileName);

                if (!fileOut.open(QIODevice::ReadWrite) )
                {
                    qDebug() << fileIn.errorString() << ": " << (path + "/" + fileName);
                    continue ;
                }

                fileOut.write(QByteArray(fileIn.readAll()));

                fileOut.close();
                fileIn.close();
            }

            dlib::object_detector<image_scanner_type> detector;

            dlib::deserialize(QString(path + "/" + fileName).toStdString().c_str()) >> detector;
            _detectors.push_back(detector);
        }
    }

    /*
     * Create threads for video computing
    */
    {
        unsigned int nthreads = std::thread::hardware_concurrency();
        std::chrono::milliseconds wait(1000 / nthreads);

        for (unsigned int i = 0; i < nthreads; i++)
        {
            _threads.push_back(std::thread(&RoadVideoFilterRunnable::_detect, this));
            std::this_thread::sleep_for(wait);
        }
    }
}

QVideoFrame RoadVideoFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags)
{
    if (!input->isValid() || !input->map(QAbstractVideoBuffer::ReadOnly))
    {
        return *input;
    }

    /*
     * Convert frame to cv format, and notify only one thread
     * Apply cv filter to frame
    */
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
    /*
     * Thread loop, run tsr and tld for catched frame
    */
    while (true)
    {
        try
        {
            while (true)
            {
                cv::Mat frameGray;

                {
                    std::unique_lock<std::mutex> lock(_frameRGBMutex);
                    _newFrame.wait(lock);
                    cv::cvtColor(_frameRGB, frameGray, CV_RGB2GRAY);
                }

                cv::Mat filter(frameGray.rows, frameGray.cols, CV_8UC3, cv::Scalar(0, 0, 0));

                _tsr(frameGray, filter);

                if (true)
                {
                    _tld(frameGray, filter);
                }

                std::unique_lock<std::mutex> lock(_filterMutex);
                _filter = filter;
            }
        }
        catch (std::exception& e)
        {
             qDebug() << "RoadVideoFilterRunnable::_detect: " << e.what();
        }
    }
}

void RoadVideoFilterRunnable::_tsr(cv::Mat frameGray, cv::Mat& filter)
{
    /*
     * Traffic sign recognititon
    */
    try
    {
        dlib::array2d<unsigned char> dlibImageGray;

        dlib::assign_image(dlibImageGray, dlib::cv_image<unsigned char>(frameGray));

        std::vector<dlib::rect_detection> rects;

        evaluate_detectors(_detectors, dlibImageGray, rects);

        {
            std::unique_lock<std::mutex> lock(_detectedSignsMutex);

            for (auto i = rects.begin(); i != rects.end(); i++)
            {
                cv::Rect rect = cv::Rect(cv::Point2i(static_cast<int>(i->rect.left()), static_cast<int>(i->rect.top())),
                                         cv::Point2i(static_cast<int>(i->rect.right()), static_cast<int>(i->rect.bottom())));
                rectangle(filter, rect,  cv::Scalar(0, 255, 0), 2);
                cv::putText(filter, signs[i->weight_index].name[0].toStdString(),
                        cv::Point(static_cast<int>(i->rect.right()), (static_cast<int>(i->rect.bottom()) + static_cast<int>(i->rect.top())) / 2),
                        cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0), 2);

                auto f = _detectedSigns.find(signs[i->weight_index].id);

                if (f != _detectedSigns.end())
                {
                    std::chrono::duration<double> diff = std::chrono::system_clock::now() - _detectedSigns[signs[i->weight_index].id];

                    if (diff.count() > 10)
                    {
                        _detectedSigns[signs[i->weight_index].id] = std::chrono::system_clock::now();
                        emit static_cast<RoadVideoFilter*>(_rvf)->newSign(signs[i->weight_index].id, signs[static_cast<std::size_t>(signs[i->weight_index].id)].name[static_cast<RoadVideoFilter*>(_rvf)->lenguageID]);
                    }
                }
                else
                {
                    _detectedSigns[signs[i->weight_index].id] = std::chrono::system_clock::now();
                    emit static_cast<RoadVideoFilter*>(_rvf)->newSign(signs[i->weight_index].id, signs[static_cast<std::size_t>(signs[i->weight_index].id)].name[static_cast<RoadVideoFilter*>(_rvf)->lenguageID]);
                }
            }
        }
    }
    catch (std::exception& e)
    {
         qDebug() << "tsr: " << e.what();
         throw ;
    }
}

void RoadVideoFilterRunnable::_tld(cv::Mat frame_gray, cv::Mat& filter)
{
    /*
     * Traffic line detection
    */
    try
    {
        int h = frame_gray.rows;
        int w = frame_gray.cols;

        cv::threshold(frame_gray, frame_gray, 140, 255, cv::THRESH_BINARY);

        cv::Mat kernel;
        cv::Point anchor;

        anchor = cv::Point(-1, -1);
        kernel = cv::Mat(1, 3, CV_32F);
        kernel.at<float>(0, 0) = -1;
        kernel.at<float>(0, 1) = 0;
        kernel.at<float>(0, 2) = 1;

        cv::filter2D(frame_gray, frame_gray, -1, kernel, anchor, 0, cv::BORDER_DEFAULT);

        cv::Mat mask = cv::Mat::zeros(frame_gray.size(), frame_gray.type());
        static const cv::Point pts[4] = {
            cv::Point(0, h),
            cv::Point(w / 2 - w / 48, h * 3 / 5),
            cv::Point(w / 2 + w / 48, h * 3 / 5),
            cv::Point(w, h)
        };

        cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255, 0, 0));

        cv::Mat img_mask;
        cv::bitwise_and(frame_gray, mask, img_mask);

        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(img_mask, lines, 1, CV_PI / 180, 20, 20, 30);

        if (!lines.empty())
        {
            std::vector<cv::Point> left_points;
            std::vector<cv::Point> right_points;

            cv::Mat line_filter(frame_gray.rows, frame_gray.cols, CV_8UC3, cv::Scalar(0, 0, 0));

            for (auto i : lines)
            {
                if (i[0] < w / 2 && i[2] < w / 2)
                {
                    left_points.push_back(cv::Point(i[0], i[1]));
                    left_points.push_back(cv::Point(i[2], i[3]));
                }
                else if (i[0] > w / 2 && i[2] > w / 2)
                {
                    right_points.push_back(cv::Point(i[0], i[1]));
                    right_points.push_back(cv::Point(i[2], i[3]));
                }
            }

            if (!left_points.empty() && !right_points.empty())
            {
                cv::Vec4f l, r;

                cv::fitLine(left_points, l, CV_DIST_L2, 0, 0.01, 0.01);
                cv::fitLine(right_points, r, CV_DIST_L2, 0, 0.01, 0.01);

                int r1 = h * 3 / 5;
                int r0 = WbyNormVec(r1, r[2], r[3], r[0], r[1]);
                int r3 = h;
                int r2 = WbyNormVec(r3, r[2], r[3], r[0], r[1]);

                int l1 = h * 3 / 5;
                int l0 = WbyNormVec(l1, l[2], l[3], l[0], l[1]);
                int l3 = h;
                int l2 = WbyNormVec(l3, l[2], l[3], l[0], l[1]);

                if (r0 > w / 2 && r2 > w / 2 && l0 < w / 2 && l2 < w / 2 && r2 > r0 && l2 < l0)
                {
                    roadDirections_t status = road_not_found;

                    double ar = fabs(atan2(r1 - r3, r0 - r2) * 180 / 3.14159265358979323846);
                    double al = 180 - fabs(atan2(l1 - l3, l0 - l2) * 180 / 3.14159265358979323846);
                    double d = 10;

                    if (ar < al + d && ar > al - d)
                    {
                        status = straight_road;
                    }
                    else if (al < ar)
                    {
                        status = left_road;
                    }
                    else
                    {
                        status = right_road;
                    }

                    emit static_cast<RoadVideoFilter*>(_rvf)->roadStatus(roadDirections[status][static_cast<RoadVideoFilter*>(_rvf)->lenguageID]);

                    std::vector<cv::Point> poly_points;
                    cv::Mat line_filter(frame_gray.rows, frame_gray.cols, CV_8UC3, cv::Scalar(0, 0, 0));

                    poly_points.push_back(cv::Point(r0, r1));
                    poly_points.push_back(cv::Point(r2, r3));
                    poly_points.push_back(cv::Point(l2, l3));
                    poly_points.push_back(cv::Point(l0, l1));
                    cv::fillConvexPoly(line_filter, poly_points, cv::Scalar(0, 255, 0), CV_AA, 0);

                    cv::line(line_filter, cv::Point(r0, r1), cv::Point(r2, r3), cv::Scalar(0, 0, 255), 5, CV_AA);
                    cv::line(line_filter, cv::Point(l0, l1), cv::Point(l2, l3), cv::Scalar(0, 0, 255), 5, CV_AA);

                    cv::addWeighted(line_filter, 0.4, filter, 1.0 - 0.4, 0, filter);
                }
            }
        }
    }
    catch (std::exception& e)
    {
        qDebug() << "tld: " << e.what();
        throw ;
    }
}
