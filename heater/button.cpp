#include "button.h"

#include "wiringpiwrapper.h"

Button::Button(int wiringPiPin, WiringPi::TriggerEdge trigger, QObject *parent)
    : wiringPiPin_(wiringPiPin), QObject{parent}
{
    WiringPi::pinMode(wiringPiPin_, WiringPi::eInput);

    auto isr = [this](){
        int level = WiringPi::digitalRead(wiringPiPin_);
        onButtonPushed(level);
    };

    WiringPi::isrFunction func = isr;
    WiringPi::wiringPiISR(wiringPiPin_, trigger, isr);
}

void Button::onButtonPushed(int level)
{
    emit buttonToggled(level);
}
