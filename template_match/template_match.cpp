#include "template_match.h"

#include "opencv2/opencv.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>

#include <QScreen>
#include <QPixmap>
#include <QImage>
#include <QSettings>
#include <QDir>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

#include <X11/Xlib.h> //-lX11
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xatom.h>
//#include <jpeglib.h> //-ljpeg


namespace plugin{

QImage cvMatToQImage(const cv::Mat &cvMat)
{
    if(cvMat.type() == CV_8UC4)
    {
        return QImage( cvMat.data,
                      cvMat.cols, cvMat.rows,
                      static_cast<int>(cvMat.step),
                      QImage::Format_ARGB32 );
    }
    else if(cvMat.type() == CV_8UC3)
    {
        return QImage(cvMat.data,
                      cvMat.cols,
                      cvMat.rows,
                      cvMat.step,
                      QImage::Format_RGB888).rgbSwapped();
    }

    return QImage();
}

cv::Mat screenshot()
{
    Display *display = XOpenDisplay((char *)0);
    Screen *scr = XDefaultScreenOfDisplay(display);
    Window root = XDefaultRootWindow(display);

    XImage *image = XGetImage(display, root, 0, 0, scr->width, scr->height, AllPlanes, ZPixmap);

    cv::Mat mat(image->height, image->width, CV_8UC3);
    cv::Mat mat2 = cv::Mat(image->height, image->width, image->bits_per_pixel > 24 ? CV_8UC4 : CV_8UC3, image->data);
    cv::cvtColor(mat2, mat, cv::COLOR_BGRA2BGR);

    XDestroyImage(image);
    XCloseDisplay(display);

    return mat;
}

cv::Mat QImageTocvMat(const QImage &image)
{
    QImage img = image.convertedTo(QImage::Format_RGB888);
    cv::Mat mat(img.height(), img.width(), CV_8UC3);
    for (int i=0;i<img.height();i++) {
        memcpy(mat.ptr(i),img.constScanLine(i),img.bytesPerLine());
    }
    cv::Mat ret(img.height(), img.width(), CV_8UC3);
    cv::cvtColor(mat, ret, cv::COLOR_RGB2BGR);
    return ret;
}

bool TemplateMatch::initialize()
{
    foundTag_ = tagList()->createTag("templatematch", "found", TagType::eBool, false, "Template found");

    QSettings settings("june", "june");
    QString dir = settings.value("plugins/templatedir").toString() + QDir::separator();
    QString file = settings.value("plugins/template").toString();
    QString path = dir + file;

    template_.load(path);
    return true;
}

void TemplateMatch::mainloop()
{
    cv::Mat target = screenshot();
    cv::Mat templa = QImageTocvMat(template_);

    cv::Mat res;
    res.create(target.cols - templa.cols + 1, target.rows - templa.rows +1, CV_8UC3);
    double min;
    double max;
    cv::Point minLoc;
    cv::Point maxLoc;
    cv::matchTemplate(target, templa, res, cv::TM_SQDIFF_NORMED);

    cv::minMaxLoc(res, &min, &max, &minLoc, &maxLoc, cv::Mat());
    qDebug() << "prop: " << 1 - min;
    bool found;
    if(auto prop =  1 - min; prop > 0.99)
    {
        found = true;
    }
    else
        found = false;

    if(found != isFound_)
    {
        if(found)
        {
            qDebug() << "found";
            foundTag_->setValue(true);
        }
        else
        {
            qDebug() << "not found";
            foundTag_->setValue(false);
        }
        isFound_ = found;
    }
}

}// end namespace
