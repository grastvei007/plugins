#include "heater.h"
#include "wiringpiwrapper.h"

#include <QDebug>

extern "C" PluginInterface* createPlugin()
{
  WiringPi::setup();
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
	qDebug() << __FUNCTION__;
//    WiringPi::setup();

    powerOnTag_ = tagList_->createTag("heater", "powerOn", Tag::eBool);
    heatLevelTag_ = tagList_->createTag("heater", "heatLevel", Tag::eInt);
    fanLevelTag_ = tagList_->createTag("heater", "fanLevel", Tag::eInt);
    stateTag_ = tagList_->createTag("heater", "state", Tag::eString);
    isBurningTag_ = tagList_->createTag("heater", "isBurning", Tag::eBool);

    stateTag_->setValue(stateToString(state_));
    powerOnTag_->setValue(powerOn_);
    heatLevelTag_->setValue(heatLevel_);
    fanLevelTag_->setValue(fanLevel_);
    isBurningTag_->setValue(isBurning_);

    powerOnTagSocket_ = TagSocket::createTagSocket("heater", "powerOn", TagSocket::eBool);
    heatLevelTagSocket_ = TagSocket::createTagSocket("heater", "heatLevel", TagSocket::eInt);
    fanLevelTagSocket_ = TagSocket::createTagSocket("heater", "fanLevel", TagSocket::eInt);
    isBurningTagSocket_ = TagSocket::createTagSocket("heater", "isBurning", TagSocket::eBool);

    powerOnTagSocket_->hookupTag(powerOnTag_);
    heatLevelTagSocket_->hookupTag(heatLevelTag_);
    fanLevelTagSocket_->hookupTag(fanLevelTag_);
    isBurningTagSocket_->hookupTag(isBurningTag_);

    connect(powerOnTagSocket_, qOverload<bool>(&TagSocket::valueChanged), this, &Heater::onPowerOnValueChanged);
    connect(heatLevelTagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Heater::onHeatLevelValueChanged);
    connect(fanLevelTagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Heater::onFanLevelValueChanged);
    connect(isBurningTagSocket_, qOverload<bool>(&TagSocket::valueChanged), this, &Heater::onIsBurningValueChanged);

    return true;
}

void Heater::run(int deltaMs)
{
    if(!mainLoopTimer_)
        mainLoopTimer_->deleteLater();
    deltaMs_ = deltaMs;

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
}

void Heater::onFanLevelValueChanged(int value)
{
    fanLevel_ = value;
}

void Heater::onIsBurningValueChanged(bool value)
{
    isBurning_ = value;
}

void Heater::onHeatLevelValueChanged(int value)
{
    heatLevel_ = value;
}

void Heater::mainloop()
{
    States currentState = state_;

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

    if(currentState != state_)
    {
        stateTag_->setValue(stateToString(state_));
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
    if(powerOn_)
    {
        preHeatTime_ = 0;
        state_ = ePreHeat;
    }
}

void Heater::statePreHeat()
{
    double deltaS = deltaMs_ / 1000.0;
    preHeatTime_ += deltaS;

    if(!pump_.isRunning())
    {
        pump_.setInterval(1);
        pump_.start();
        pump_.setSpeed(100);
    }

    if(!preHeatUnit_.isActive() && preHeatTime_ > 10.0)
    {
        preHeatUnit_.setActive(true);
        motorHeat_.turnOn();
        motorHeat_.setSpeed(80);
    }


    if(preHeatTime_ > 30.0)
    {
        state_ = eStarting;
    }

    if(!powerOn_)
    {
        stoppingTime_ = 0;
        state_ = eStoping;
    }
}

void Heater::stateStarting()
{
    if(preHeatUnit_.isActive())
        preHeatUnit_.setActive(false);

    if(isBurning_)
    {
        motorFan_.turnOn();
        motorFan_.setSpeed(50);
        heatLevel_ = 50;
        fanLevel_ = 50;
        currentHeatLevel_ = heatLevel_;
        currentFanLevel_ = fanLevel_;
        heatLevelTagSocket_->writeValue(heatLevel_);
        fanLevelTagSocket_->writeValue(fanLevel_);

        state_ = eRunning;
    }

    if(!powerOn_)
    {
        stoppingTime_ = 0;
        state_ = eStoping;
    }
}

void Heater::stateRunning()
{

    if(currentHeatLevel_ != heatLevel_)
    {
        pump_.setSpeed(heatLevel_);
        motorHeat_.setSpeed(heatLevel_);

        currentHeatLevel_ = heatLevel_;
    }

    if(currentFanLevel_ != fanLevel_)
    {
        motorFan_.setSpeed(fanLevel_);
        currentFanLevel_ = fanLevel_;
    }

    if(!powerOn_)
    {
        stoppingTime_ = 0;
        state_ = eStoping;
    }
}

void Heater::stateStopping()
{
    stoppingTime_ += deltaMs_ / 1000.0;
    preHeatUnit_.setActive(false);
	pump_.stop();

    if(stoppingTime_ > 300)
    {
        motorFan_.turnOff();
        powerOn_ = false;
        powerOnTagSocket_->writeValue(powerOn_);
        state_ = eOff;
        isBurning_ = false;
    }

}
