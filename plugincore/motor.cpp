#include "motor.h"

#include <QTimer>
#include "wiringpiwrapper.h"

namespace core{

Motor::Motor(int wireingPiPin, QObject *parent) :
    wireingPiPin_(wireingPiPin),
    QObject(parent)
{

}

void Motor::setSpeed(int speed)
{
    targetSpeed_ = std::clamp(speed, min_, maxSpeed_);
    adjustSpeed();
}

void Motor::setMaximumSpeed(int max)
{
    maxSpeed_ = std::clamp(max, min_, max_);
}

void Motor::stop()
{
    setSpeed(0);
}

bool Motor::isRunning() const
{
    return currentSpeed_ > 0;
}

void Motor::adjustSpeed()
{
    auto adjust = targetSpeed_ - currentSpeed_;
    if(adjust == 0)
        return;
    else if(adjust < 0)
        currentSpeed_++;
    else
        currentSpeed_--;

    WiringPi::softPwmWrite(wireingPiPin_, currentSpeed_);
    QTimer::singleShot(rampUpTime_, this, &Motor::adjustSpeed);
}

} // end namespace
