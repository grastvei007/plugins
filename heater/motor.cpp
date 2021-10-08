#include "motor.h"

#include "wiringpiwrapper.h"

Motor::Motor(int wireingPiPin) : wireingPiPin_(wireingPiPin)
{
    WiringPi::softPwmCreate(wireingPiPin_, 0, 100);
}

void Motor::turnOn()
{
    isOn_ = true;
    setSpeed(currentSpeed_);
}

void Motor::turnOff()
{
    isOn_ = false;
    WiringPi::softPwmWrite(wireingPiPin_, 0);
}

void Motor::setSpeed(int speed)
{
    if(!isOn())
    {
        currentSpeed_ = 0;
    }
    if(speed < minSpeed_)
        speed = minSpeed_;
    else if(speed > maxSpeed_)
        speed = maxSpeed_;

    currentSpeed_ = speed;

    WiringPi::softPwmWrite(wireingPiPin_, currentSpeed_);
}

bool Motor::isRunning() const
{
    return !(currentSpeed_ == 0);
}

bool Motor::isOn() const
{
    return isOn_;
}
