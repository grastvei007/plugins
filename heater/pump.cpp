#include "pump.h"

Pump::Pump()
{

}

void Pump::setInterval(double intervalInSeconds)
{
    pumpIntervalSeconds_ = intervalInSeconds;
}

void Pump::setSpeed(int speed)
{

}

void Pump::start()
{

}

void Pump::stops()
{

}

bool Pump::isRunning() const
{
    return running_;
}
