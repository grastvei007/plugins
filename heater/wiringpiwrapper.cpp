#include "wiringpiwrapper.h"

#ifdef RASPBERRY_PI
#include <wiringPi.h>
#include <softPwm.h>
#else
#include <QDebug>
#endif



void WiringPi::wiringPiSetup()
{
#ifdef RASPBERRY_PI
    ::wiringPiSetup();
#else
    qDebug() << __FUNCTION__;
#endif
}

void WiringPi::pinMode(int pin, PinDir dir)
{
#ifdef RASPBERRY_PI
    if(aDir == eInput)
        ::pinMode(aPin, 0);
    else if(aDir == eOutput)
        ::pinMode(aPin, 1);
#else
    Q_UNUSED(pin);
    Q_UNUSED(dir);
    qDebug() << __FUNCTION__;
#endif

}


void WiringPi::digitalWrite(int pin, Value val)
{
#ifdef RASPBERRY_PI
    if(aVal == eLow)
        ::digitalWrite(aPin, 0);
    else if(aVal == eHigh)
        ::digitalWrite(aPin, 1);
#else
    Q_UNUSED(pin);
    Q_UNUSED(val);
    qDebug() << __FUNCTION__;
#endif
}


int WiringPi::softPwmCreate(int pin, int initVal, int pwmRange)
{
#ifdef RASPBERRY_PI
      return ::softPwmCreate(aPin, aInitVal, aPwmRange);
#else
    Q_UNUSED(pin);
    Q_UNUSED(initVal);
    Q_UNUSED(pwmRange);
    qDebug() << __FUNCTION__;
    return 0;
#endif
}


void WiringPi::softPwmWrite(int pin, int value)
{
#ifdef RASPBERRY_PI
    ::softPwmWrite(aPin, aValue);
#else
    Q_UNUSED(pin);
    Q_UNUSED(value);
    qDebug() << __FUNCTION__;
#endif
}


void WiringPi::pwmSetClock(int clock)
{
#ifdef RASPBERRY_PI
    ::pwmSetClock(aClock);
#else
    Q_UNUSED(clock);
    qDebug() << __FUNCTION__;
#endif
}


void WiringPi::pwmSetRange(int range)
{
#ifdef RASPBERRY_PI
    ::pwmSetRange(aRange);
#else
    Q_UNUSED(range);
    qDebug() << __FUNCTION__;
#endif
}


void WiringPi::pwmSetMode(PwmMode pwmMode)
{
#ifdef RASPBERRY_PI
    ::pwmSetMode(aPwmMode == ePwmModeMs ? 0 : 1);
#else
    Q_UNUSED(pwmMode);
    qDebug() << __FUNCTION__;
#endif
}
