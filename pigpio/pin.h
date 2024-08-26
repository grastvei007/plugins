#ifndef PIN_H
#define PIN_H

#include <QObject>

#include <tagsystem/tagsocket.h>

#include "wiringpiwrapper.h"

class Pin : public QObject
{
    Q_OBJECT
public:
    Pin(TagSocket *tagSocket, int pinNumber, WiringPi::PinDir dir);

private slots:
    void onValueChanged(int value);
    void onPwmValueChanged(int value);

private:
    TagSocket *tagSocket_;
    int pinNumber_;
    WiringPi::PinDir direction_ = WiringPi::PinDir::eOutput;
    WiringPi::Value value_ = WiringPi::eLow;
    int pwmValue_ = 0;
};

#endif // PIN_H
