#include "heater.h"

extern "C" PluginInterface* createPlugin()
{
  return new Heater();
}


Heater::Heater()
{

}

void Heater::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

bool Heater::initialize()
{
    powerOnTag_ = tagList_->createTag("heater", "powerOn", Tag::eBool);
    heatLevelTag_ = tagList_->createTag("heater", "heatLevel", Tag::eInt);
    stateTag_ = tagList_->createTag("heater", "state", Tag::eString);
    stateTag_->setValue(stateToString(state_));

    powerOnTagSocket_ = TagSocket::createTagSocket("heater", "powerOn", TagSocket::eBool);
    heatLevelTagSocket_ = TagSocket::createTagSocket("heater", "heatLevel", TagSocket::eInt);

    powerOnTagSocket_->hookupTag(powerOnTag_);
    heatLevelTagSocket_->hookupTag(heatLevelTag_);

    connect(powerOnTagSocket_, qOverload<bool>(&TagSocket::valueChanged), this, &Heater::onPowerOnValueChanged);
    connect(heatLevelTagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Heater::onHeatLevelValueChanges);
    return true;
}

void Heater::run(int deltaMs)
{
    if(!mainLoopTimer_)
        mainLoopTimer_->deleteLater();

    mainLoopTimer_ = new QTimer();
    mainLoopTimer_->setInterval(deltaMs);
    QObject::connect(mainLoopTimer_, &QTimer::timeout, this, &Heater::mainloop);

    mainLoopTimer_->start();
}

void Heater::stop()
{

}

void Heater::onPowerOnValueChanged(bool value)
{
    powerOn_ = value;
    if(state_ == eOff && powerOn_)
        state_ = ePreHeat;
}

void Heater::onHeatLevelValueChanges(int value)
{
    heatLevel_ = value;
}

void Heater::mainloop()
{
    switch (state_)
    {
        case eOff:
            stateOff();
            break;
        case ePreHeat:
            statePreHeat();
            break;
        case eStarting:
            stateStarting();
            break;
        case eRunning:
            stateRunning();
            break;
        case eStoping:
            stateStopping();
            break;
    }
}

QString Heater::stateToString(Heater::States state)
{
    if(state == eOff)
        return "Off";
    else if(state == ePreHeat)
        return "Preheat";
    else if(state == eStarting)
        return "Starting";
    else if(state == eRunning)
        return "Running";
    else if(state == eStoping)
        return "Stopping";

    return QString();
}

void Heater::stateOff()
{

}

void Heater::statePreHeat()
{

}

void Heater::stateStarting()
{

}

void Heater::stateRunning()
{

}

void Heater::stateStopping()
{

}
