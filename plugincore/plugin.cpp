#include "plugin.h"

#include <QDebug>

Plugin::Plugin(const QString &subsystem) :
    subsystem_(subsystem)
{

}

void Plugin::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

void Plugin::createApi(QHttpServer &)
{
    // default falltrough
    return;
}

bool Plugin::initialize()
{
    qDebug() << "overide to setup the plugin";
    return false;
}

void Plugin::run(int deltaMs)
{
    if(mainLoopTimer_)
        mainLoopTimer_.release();

	deltaMs_ = deltaMs;
	mainLoopTimer_ = std::make_unique<QTimer>(this);
	mainLoopTimer_->setInterval(deltaMs);
	QObject::connect(mainLoopTimer_.get(), &QTimer::timeout, this, &Plugin::mainloop);

    mainLoopTimer_->start();
}

void Plugin::stop()
{
    mainLoopTimer_->stop();
}

TagList *Plugin::tagList() const
{
    return tagList_;
}

int Plugin::runTimeStep() const
{
	return deltaMs_;
}

QString Plugin::subsystem() const
{
    return subsystem_;
}

void Plugin::mainloop()
{
    qDebug() << __FUNCTION__ << " overide this to make a mainloop for your plugin";
}
