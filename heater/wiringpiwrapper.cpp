#include "wiringpiwrapper.h"

#ifdef RASPBERRY_PI
#include <wiringPi.h>
#include <softPwm.h>
#else
#include <QDebug>
#include <QString>
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
    QString value = (dir == eInput) ? "input" : "output";
    qDebug() << __FUNCTION__ << " Pin=" << pin << " dir=" << value;
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
    QString value = (val == eLow) ? "Low" : "High";
    qDebug() << __FUNCTION__ << " Pin=" << pin << " " << value;
#endif
}


int WiringPi::softPwmCreate(int pin, int initVal, int pwmRange)
{
#ifdef RASPBERRY_PI
      return ::softPwmCreate(aPin, aInitVal, aPwmRange);
#else
    qDebug() << __FUNCTION__ << " pin=" << pin << " initVal=" << initVal << " pwmRange=" << pwmRange;
    return 0;
#endif
}


void WiringPi::softPwmWrite(int pin, int value)
{
#ifdef RASPBERRY_PI
    ::softPwmWrite(aPin, aValue);
#else
    qDebug() << __FUNCTION__ << " pin=" << pin << " value=" << value;
#endif
}


void WiringPi::pwmSetClock(int clock)
{
#ifdef RASPBERRY_PI
    ::pwmSetClock(aClock);
#else
    qDebug() << __FUNCTION__ << " clock=" << clock;
#endif
}


void WiringPi::pwmSetRange(int range)
{
#ifdef RASPBERRY_PI
    ::pwmSetRange(aRange);
#else
    qDebug() << __FUNCTION__ << " range=" << range;
#endif
}


void WiringPi::pwmSetMode(PwmMode pwmMode)
{
#ifdef RASPBERRY_PI
    ::pwmSetMode(aPwmMode == ePwmModeMs ? 0 : 1);
#else
    qDebug() << __FUNCTION__ << " pwmMode=" << pwmMode;
#endif
}
