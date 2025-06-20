#include "pin.h"

#include <functional>
#include <QThread>
#include <QJsonObject>

namespace plugin{

std::map<int,std::function<void(int)>> functions; // gpio, level

Pin::Pin(TagSocket *tagSocket, Tag *tag, int pinNumber, WiringPi::PinDir dir) :
    tagSocket_(tagSocket),
    tag_(tag),
    pinNumber_(pinNumber),
    direction_(dir)
{
    setupPin();
}

void Pin::setEnable(bool enable)
{
    if (enabled_ == enable)
        return;

    if(!enable)
        onValueChanged(0);
    enabled_ = enable;
}

void Pin::setDirection(WiringPi::PinDir dir)
{
    if (direction_ == dir)
        return;

    disconnect(tagSocket_, nullptr, this, nullptr);
    functions.erase(pinNumber_);
    direction_ = dir;
    setupPin();
}

QJsonObject Pin::toJson() const
{
    QJsonObject object;

    object.insert("enabled", enabled_);
    object.insert("subsystem", tag_->getSubsystem());
    object.insert("tag", tag_->getName());
    object.insert("description", tag_->getDescription());
    object.insert("wiringpi", pinNumber_);
    object.insert("dir", QString::number(direction_));

    return object;
}

int Pin::wiringPiPin() const
{
    return pinNumber_;
}

void Pin::onValueChanged(int value)
{
    if (!enabled_)
        return;

    if(direction_ == WiringPi::eOutput)
        value = std::clamp(0, 1, value);
    else if(direction_ == WiringPi::ePwm)
        value = std::clamp(0, 100, value);
    else
        return;

    if(value == value_)
        return;

    value_ = value;
    if(direction_ == WiringPi::eOutput)
    {
        WiringPi::digitalWrite(pinNumber_, static_cast<WiringPi::Value>(value_));
    }
    else // pwm
        WiringPi::softPwmWrite(pinNumber_, value_);
}

void Pin::digitalRead(int value)
{
    if(currentReadValue_ == value)
        return;
    currentReadValue_ = value;
    tag_->setValue(currentReadValue_);
    QThread::msleep(50); // avoid bouncing signal
}

void Pin::setupCallback()
{
    if(pinNumber_ == 29)
    {
        auto isr29 = +[](){
            int level = WiringPi::digitalRead(29);
            functions[29](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr29);
    }
    else if(pinNumber_ == 28)
    {
        auto isr28 = +[](){
            int level = WiringPi::digitalRead(28);
            functions[28](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr28);
    }
    else if(pinNumber_ == 27)
    {
        auto isr27 = +[](){
            int level = WiringPi::digitalRead(27);
            functions[27](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr27);
    }
    else if(pinNumber_ == 26)
    {
        auto isr26 = +[](){
            int level = WiringPi::digitalRead(26);
            functions[26](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr26);
    }
    else if(pinNumber_ == 25)
    {
        auto isr25 = +[](){
            int level = WiringPi::digitalRead(25);
            functions[25](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr25);
    }
    else if(pinNumber_ == 24)
    {
        auto isr24 = +[](){
            int level = WiringPi::digitalRead(24);
            functions[24](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr24);
    }
    else if(pinNumber_ == 23)
    {
        auto isr23 = +[](){
            int level = WiringPi::digitalRead(23);
            functions[23](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr23);
    }
    else if(pinNumber_ == 22)
    {
        auto isr22 = +[](){
            int level = WiringPi::digitalRead(22);
            functions[22](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr22);
    }
    else if(pinNumber_ == 21)
    {
        auto isr21 = +[](){
            int level = WiringPi::digitalRead(21);
            functions[21](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr21);
    }
    else if(pinNumber_ == 7)
    {
        auto isr7 = +[](){
            int level = WiringPi::digitalRead(7);
            functions[7](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr7);
    }
    else if(pinNumber_ == 6)
    {
        auto isr6 = +[](){
            int level = WiringPi::digitalRead(6);
            functions[6](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr6);
    }
    else if(pinNumber_ == 5)
    {
        auto isr5 = +[](){
            int level = WiringPi::digitalRead(5);
            functions[5](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr5);
    }
    else if(pinNumber_ == 4)
    {
        auto isr4 = +[](){
            int level = WiringPi::digitalRead(4);
            functions[4](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr4);
    }
    else if(pinNumber_ == 3)
    {
        auto isr3 = +[](){
            int level = WiringPi::digitalRead(3);
            functions[3](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr3);
    }
    else if(pinNumber_ == 2)
    {
        auto isr2 = +[](){
            int level = WiringPi::digitalRead(2);
            functions[2](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr2);
    }
    else if(pinNumber_ == 1)
    {
        auto isr1 = +[](){
            int level = WiringPi::digitalRead(1);
            functions[1](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr1);
    }
    else if(pinNumber_ == 0)
    {
        auto isr0 = +[](){
            int level = WiringPi::digitalRead(0);
            functions[0](level);
        };
        WiringPi::wiringPiISR(pinNumber_, WiringPi::TriggerEdge::eINT_EDGE_RISING, isr0);
    }
}

void Pin::setupPin()
{
    if (direction_ == WiringPi::eOutput)
    {
        WiringPi::pinMode(pinNumber_, WiringPi::eOutput);
        connect(tagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Pin::onValueChanged);
        WiringPi::digitalWrite(pinNumber_, static_cast<WiringPi::Value>(value_));
    } else if (direction_ == WiringPi::eInput)
    {
        WiringPi::pinMode(pinNumber_, WiringPi::eInput);
        int currentReadValue_ = WiringPi::digitalRead(pinNumber_);
        tag_->setValue(currentReadValue_);
        //
        setupCallback();
        functions[pinNumber_] = std::bind(&Pin::digitalRead, this, std::placeholders::_1);
    } else if (direction_ == WiringPi::ePwm)
    {
        WiringPi::softPwmCreate(pinNumber_, 0, 100);
        connect(tagSocket_, qOverload<int>(&TagSocket::valueChanged), this, &Pin::onValueChanged);
        WiringPi::softPwmWrite(pinNumber_, 0);
    }
}

}//end namespace
