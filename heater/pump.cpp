#include "pump.h"

#include "wiringpiwrapper.h"
#include <QDebug>

Pump::Pump(int wireingPiPin) : wireingpiPin_(wireingPiPin), pumpTimer_()
{
    WiringPi::pinMode(wireingpiPin_, WiringPi::eOutput);
    connect(&pumpTimer_, &QTimer::timeout, this, &Pump::onPumpTimerTimeout);
    running_ = false;
}

Pump::Pump(const Pump &pump) :
    pumpTimer_()
{
    currentSpeed_ = pump.currentSpeed_;
    pumpIntervalSeconds_ = pump.pumpIntervalSeconds_;
    pumpMinInterValSeconds_ = pump.pumpMinInterValSeconds_;
    pumpMaxIntervalSeconds_ = pump.pumpMaxIntervalSeconds_;
    running_ = pump.running_;

    wireingpiPin_ = pump.wireingpiPin_;

    WiringPi::pinMode(wireingpiPin_, WiringPi::eOutput);
    connect(&pumpTimer_, &QTimer::timeout, this, &Pump::onPumpTimerTimeout);
}

Pump::~Pump()
{

}

void Pump::setInterval(double intervalInSeconds)
{
    pumpIntervalSeconds_ = intervalInSeconds;
}

void Pump::setSpeed(int speed)
{
    if(speed < 0)
        speed = 0;
    else if(speed > 100)
        speed = 100;
    currentSpeed_ = speed;

    double val = (pumpMaxIntervalSeconds_ - pumpMinInterValSeconds_) * (speed) / 100.0;
    val = pumpMaxIntervalSeconds_ - val;

    int setPoint = val * 1000;

    pumpTimer_.stop();
    if(speed == 0 || !isRunning())
        return;
    pumpTimer_.setInterval(1000 + setPoint);
    pumpTimer_.start();
}

void Pump::start()
{
    running_ = true;
    setSpeed(currentSpeed_);
}

void Pump::stop()
{
    running_ = false;
    pumpTimer_.stop();
}

bool Pump::isRunning() const
{
    return running_;
}

void Pump::onPumpTimerTimeout()
{
    //turn pump on
    WiringPi::digitalWrite(wireingpiPin_, WiringPi::eHigh);
    // turn pump off
    QTimer::singleShot(400, [&](){
        WiringPi::digitalWrite(wireingpiPin_, WiringPi::eLow);
    });
}
