#include "button.h"
#include <functional>
#include "wiringpiwrapper.h"
#include <QDebug>
#include <QThread>

std::map<int,std::function<void(int)>> functions; // gpio, level


Button::Button(int wiringPiPin, WiringPi::TriggerEdge trigger, QObject *parent)
    : wiringPiPin_(wiringPiPin), trigger_(trigger), QObject{parent}
{
    WiringPi::pinMode(wiringPiPin_, WiringPi::eInput);
    setupCallback();
    functions[wiringPiPin_] = std::bind(&Button::onButtonPushed, this, std::placeholders::_1);
}

void Button::onButtonPushed(int level)
{
    if(state_ == level)
        return;
    state_ = level;
    qDebug() << __FUNCTION__ << wiringPiPin_ << " " << level;
    emit buttonToggled(level);
    QThread::msleep(50);
}

void Button::setupCallback()
{
    if(wiringPiPin_ == 29)
    {
        auto isr29 = +[](){
            int level = WiringPi::digitalRead(29);
            functions[29](level);
        };
        WiringPi::wiringPiISR(wiringPiPin_, trigger_, isr29);
    }
    else if(wiringPiPin_ == 28)
    {
        auto isr28 = +[](){
            int level = WiringPi::digitalRead(28);
            functions[28](level);
        };
        WiringPi::wiringPiISR(wiringPiPin_, trigger_, isr28);
    }
    else if(wiringPiPin_ == 27)
    {
        auto isr27 = +[](){
            int level = WiringPi::digitalRead(27);
            functions[27](level);
        };
        WiringPi::wiringPiISR(wiringPiPin_, trigger_, isr27);
    }
    else if(wiringPiPin_ == 26)
    {
        auto isr26 = +[](){
            int level = WiringPi::digitalRead(26);
            functions[26](level);
        };
        WiringPi::wiringPiISR(wiringPiPin_, trigger_, isr26);
    }
    else if(wiringPiPin_ == 25)
    {
        auto isr25 = +[](){
            int level = WiringPi::digitalRead(25);
            functions[25](level);
        };
        WiringPi::wiringPiISR(wiringPiPin_, trigger_, isr25);
    }
}
