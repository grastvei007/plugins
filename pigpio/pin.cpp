#include "pin.h"



Pin::Pin(TagSocket *tagSocket, int pinNumber, int mode) :
    tagSocket_(tagSocket),
    pinNumber_(pinNumber),
    mode_(mode)
{
    if(mode_ == WiringPi::eOutput)
    {
        WiringPi::pinMode(pinNumber_, WiringPi::eOutput);
        connect(tagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Pin::onValueChanged);
        WiringPi::digitalWrite(pinNumber_, value_);
    }
    else if(mode_ == 3)
    {
        WiringPi::pinMode(pinNumber_, WiringPi::eOutput);
        WiringPi::softPwmCreate(pinNumber_, 0, 100);
        connect(tagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Pin::onPwmValueChanged);
        WiringPi::softPwmWrite(pinNumber_, pwmValue_);
    }
}

void Pin::onValueChanged(int value)
{
    auto val = static_cast<WiringPi::Value>(value);
    if( val == value_)
        return;

    value_ = val;
    WiringPi::digitalWrite(pinNumber_, value_);
}

void Pin::onPwmValueChanged(int value)
{
    if(value == pwmValue_)
        return;
    pwmValue_ = std::clamp(value, 0, 100);
    WiringPi::softPwmWrite(pinNumber_, pwmValue_);
}
