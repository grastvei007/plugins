#include "pump.h"

#include "wiringpiwrapper.h"

Pump::Pump(int wireingPiPin) : wireingpiPin_(wireingPiPin)
{
    WiringPi::pinMode(wireingpiPin_, WiringPi::eOutput);
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

    double stepSize = (pumpMaxIntervalSeconds_ - pumpMinInterValSeconds_) / 100.0;
    double val = speed * stepSize;

    int timeRangeMs = (pumpMaxIntervalSeconds_ - pumpMinInterValSeconds_) * 1000;
    int setPoint = timeRangeMs * val;

    if(!pumpTimer_)
    {
        pumpTimer_ = new QTimer;
        connect(pumpTimer_, &QTimer::timeout, this, &Pump::onPumpTimerTimeout);
    }

    pumpTimer_->stop();
    if(speed == 0 || !isRunning())
        return;

    pumpTimer_->setInterval(1000 + setPoint);
    pumpTimer_->start();
}

void Pump::start()
{
    running_ = true;
    if(pumpTimer_)
        setSpeed(currentSpeed_);
}

void Pump::stop()
{
    running_ = false;
    if(pumpTimer_)
        pumpTimer_->stop();
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
