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
    configStartingTimeTag_ = tagList_->createTag("config",
                                                 "Heater_startingTime",
                                                 Tag::eInt, 0,
                                                 "Maximum time in starting state");

    powerOnTag_ = tagList_->createTag("heater", "powerOn", Tag::eBool, false);
    heatLevelTag_ = tagList_->createTag("heater", "heatLevel", Tag::eInt, 50, "Burner fan speed");
    fanLevelTag_ = tagList_->createTag("heater", "fanLevel", Tag::eInt, 50, "Air fan speed");
    stateTag_ = tagList_->createTag("heater", "state", Tag::eString);
    isBurningTag_ = tagList_->createTag("heater", "isBurning", Tag::eBool);
    wakeUpTag_ = tagList_->createTag("heater", "wake up once", Tag::eBool);
    wakeUpTimeTag_ = tagList_->createTag("heater", "wake up time", Tag::eTime);

    configStartingTimeTag_->setValue(configStartingTimeSeconds_);
    stateTag_->setValue(stateToString(state_));
    powerOnTag_->setValue(powerOn_);
    heatLevelTag_->setValue(heatLevel_);
    fanLevelTag_->setValue(fanLevel_);
    isBurningTag_->setValue(isBurning_);
    wakeUpTag_->setValue(wakeUp_);
    wakeUpTimeTag_->setValue(wakeUpTime_);

    powerOnTagSocket_ = TagSocket::createTagSocket("heater", "powerOn", TagSocket::eBool);
    heatLevelTagSocket_ = TagSocket::createTagSocket("heater", "heatLevel", TagSocket::eInt);
    fanLevelTagSocket_ = TagSocket::createTagSocket("heater", "fanLevel", TagSocket::eInt);
    isBurningTagSocket_ = TagSocket::createTagSocket("heater", "isBurning", TagSocket::eBool);
    wakeUpTagSocket_ = TagSocket::createTagSocket("heater", "wake up", TagSocket::eBool);
    wakeUpTimeTagSocket_ = TagSocket::createTagSocket("heater", "wake up time", TagSocket::eTime);
    configStartingTimeTagSocket_ = TagSocket::createTagSocket("config",
                                                              "heater_startingTime",
                                                              TagSocket::eInt);

    powerOnTagSocket_->hookupTag(powerOnTag_);
    heatLevelTagSocket_->hookupTag(heatLevelTag_);
    fanLevelTagSocket_->hookupTag(fanLevelTag_);
    isBurningTagSocket_->hookupTag(isBurningTag_);
    wakeUpTagSocket_->hookupTag(wakeUpTag_);
    wakeUpTimeTagSocket_->hookupTag(wakeUpTimeTag_);
    configStartingTimeTagSocket_->hookupTag(configStartingTimeTag_);

    connect(powerOnTagSocket_, qOverload<bool>(&TagSocket::valueChanged), this, &Heater::onPowerOnValueChanged);
    connect(heatLevelTagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Heater::onHeatLevelValueChanged);
    connect(fanLevelTagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Heater::onFanLevelValueChanged);
    connect(isBurningTagSocket_, qOverload<bool>(&TagSocket::valueChanged), this, &Heater::onIsBurningValueChanged);
    connect(wakeUpTagSocket_,
            qOverload<bool>(&TagSocket::valueChanged),
            this,
            &Heater::onWakeUpValueChanged);
    connect(wakeUpTimeTagSocket_,
            qOverload<QDateTime>(&TagSocket::valueChanged),
            this,
            &Heater::onWakeUpTimeValueChanged);

    connect(configStartingTimeTagSocket_,
            qOverload<int>(&TagSocket::valueChanged),
            this,
            &Heater::onConfigStartingTimeValueChanged);

    //buttons
    connect(&buttonHeatDown_, &Button::buttonToggled, this, &Heater::onButtonHeatDownPushed);
    connect(&buttonHeatUp_, &Button::buttonToggled, this, &Heater::onButtonHeatUpPushed);
    connect(&buttonPower_, &Button::buttonToggled, this, &Heater::onButtonPower);

    return true;
}

void Heater::run(int deltaMs)
{
    if(mainLoopTimer_)
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

void Heater::onWakeUpValueChanged(bool value)
{
    wakeUp_ = value;
}

void Heater::onWakeUpTimeValueChanged(QDateTime value)
{
    wakeUpTime_ = value;
}

void Heater::onConfigStartingTimeValueChanged(int value)
{
    configStartingTimeSeconds_ = value;
}

void Heater::onButtonHeatDownPushed(int level)
{
    if(level == 0 && heatLevel_ > 35)
        heatLevelTag_->setValue(heatLevel_-5);
}

void Heater::onButtonHeatUpPushed(int level)
{
    if(level == 0 && heatLevel_ < 100)
        heatLevelTag_->setValue(heatLevel_+5);
}

void Heater::onButtonPower(int level)
{
    if(level == 0)
        powerOnTag_->setValue(!powerOn_);
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
        startingTime_ = 0;
        state_ = ePreHeat;
        // start motot a little bit before pumping fuel
        motorHeat_.turnOn();
        motorHeat_.setSpeed(80);
        heatLevelTagSocket_->writeValue(80);
        isBurningTagSocket_->writeValue(false);
    }

    if (wakeUp_) {
        auto wakeupTime = wakeUpTime_.time().msecsSinceStartOfDay();
        auto currentTime = QDateTime::currentDateTime().time().msecsSinceStartOfDay();

        if (currentTime > wakeupTime) {
            powerOnTagSocket_->writeValue(true);
            // wake up once prevent this from activate until it is reactivatd.
            wakeUpTagSocket_->writeValue(false);
        }
    }
}

void Heater::statePreHeat()
{
    double deltaS = deltaMs_ / 1000.0;
    preHeatTime_ += deltaS;

    if(!pump_.isRunning() && preHeatTime_ > 3.0)
    {
        pump_.setInterval(1);
        pump_.start();
        pump_.setSpeed(100);
    }

    if(!preHeatUnit_.isActive() && preHeatTime_ > 10.0)
    {
        preHeatUnit_.setActive(true);
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
    double delta = deltaMs_ / 1000.0;
    startingTime_ += delta;

    if(isBurning_)
    {
		preHeatUnit_.setActive(false);
        motorFan_.turnOn();
        motorFan_.setSpeed(70);
        heatLevel_ = 70;
        fanLevel_ = 70;
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

    // automatic start after 3 min
    if (startingTime_ > configStartingTimeSeconds_)
        isBurningTagSocket_->writeValue(true);
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

        // set maximum speed while heater burn out
        fanLevelTagSocket_->writeValue(100);
        heatLevelTagSocket_->writeValue(100);
    }
}

void Heater::stateStopping()
{
    stoppingTime_ += deltaMs_ / 1000.0;
    preHeatUnit_.setActive(false);
	pump_.stop();

    if(stoppingTime_ > 360)
    {
        motorFan_.turnOff();
		motorHeat_.turnOff();
        powerOnTagSocket_->writeValue(false);
        state_ = eOff;
        isBurningTagSocket_->writeValue(false);
        heatLevelTagSocket_->writeValue(0);
        fanLevelTagSocket_->writeValue(0);
    }

}
