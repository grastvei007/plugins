#include "plugin.h"

#include <QDebug>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

Plugin::Plugin(const QString &subsystem) :
    subsystem_(subsystem)
{

}

void Plugin::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;

    // create state tag for the plugin
    stateTag_ = tagList_->createTag(subsystem(), "state", TagType::eString);
    setState(PluginState::eStopped);
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
    setState(PluginState::eRunning);
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

void Plugin::setState(PluginState state)
{
    if (stateTag_)
    {
        if (state == PluginState::eStopped)
            stateTag_->setValue("stopped");
        else if (state == PluginState::eRunning)
            stateTag_->setValue("running");
    }
}

void Plugin::mainloop()
{
    qDebug() << __FUNCTION__ << " overide this to make a mainloop for your plugin";
}
