#ifndef PIN_H
#define PIN_H

#include <QObject>

#include <tagsystem/tagsocket.h>

#include "wiringpiwrapper.h"

class Pin : public QObject
{
    Q_OBJECT
public:
    Pin(TagSocket *tagSocket, int pinNumber, int mode);

private slots:
    void onValueChanged(int value);
    void onPwmValueChanged(int value);

private:
    TagSocket *tagSocket_;
    int pinNumber_;
    int mode_;
    WiringPi::Value value_ = WiringPi::eLow;
    int pwmValue_ = 0;
};

#endif // PIN_H
