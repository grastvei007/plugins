#include "motor.h"

Motor::Motor()
{

}

void Motor::turnOn()
{
    isOn_ = true;
}

void Motor::turnOff()
{
    isOn_ = false;
}

void Motor::setSpeed(int speed)
{
    if(!isOn())
    {
        currentSpeed_ = 0;
    }
}

bool Motor::isRunning() const
{
    return !(currentSpeed_ == 0);
}

bool Motor::isOn() const
{
    return isOn_;
}
