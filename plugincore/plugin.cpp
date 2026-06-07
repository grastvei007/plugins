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
    stateTag_ = tagList_->createTag(subSystem(), "state", TagType::eString);
    setState(PluginState::eStopped);

    startTag_ = tagList_->createTag(subSystem(), "start", TagType::eBool, false);
    stopTag_ = tagList_->createTag(subSystem(), "stop", TagType::eBool, false);

    connect(startTag_, &Tag::valueChanged, this, &Plugin::onStartTagValueChanged);
    connect(stopTag_, &Tag::valueChanged, this, &Plugin::onStopTagValueChanged);
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
    stateTag_->setValue("stopped");
}

TagList *Plugin::tagList() const
{
    return tagList_;
}

int Plugin::runTimeStep() const
{
	return deltaMs_;
}

const QString& Plugin::subSystem() const
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

void Plugin::onStartTagValueChanged(Tag *tag)
{
    bool value = tag->getBoolValue();
    if (!value)
        return;

    if (mainLoopTimer_.get())
    {
        mainLoopTimer_->start();
        setState(PluginState::eRunning);

    } else
    {
        run();
    }

    QTimer::singleShot(3000, this, [&]() { startTag_->setValue(false); });
}

void Plugin::onStopTagValueChanged(Tag *tag)
{
    bool value = tag->getBoolValue();
    if (!value)
        return;

    if (mainLoopTimer_.get())
    {
        mainLoopTimer_->stop();
        setState(PluginState::eStopped);
        QTimer::singleShot(3000, this, [&]() { stopTag_->setValue(false); });
    }
}
