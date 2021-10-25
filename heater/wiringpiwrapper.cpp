#include "wiringpiwrapper.h"

#ifdef RASPBERRY_PI
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#else
#include <QDebug>
#include <QString>
#endif



void WiringPi::setup()
{
#ifdef RASPBERRY_PI
	std::cerr << __FUNCTION__ << std::endl;
    wiringPiSetup();
#else
    qDebug() << __FUNCTION__;
#endif
}

void WiringPi::pinMode(int pin, PinDir dir)
{
#ifdef RASPBERRY_PI
    if(dir == eInput)
        ::pinMode(pin, 0);
    else if(dir == eOutput)
        ::pinMode(pin, 1);
#else
    QString value = (dir == eInput) ? "input" : "output";
    qDebug() << __FUNCTION__ << " Pin=" << pin << " dir=" << value;
#endif

}


void WiringPi::digitalWrite(int pin, Value val)
{
#ifdef RASPBERRY_PI
    if(val == eLow)
        ::digitalWrite(pin, 0);
    else if(val == eHigh)
        ::digitalWrite(pin, 1);
#else
    QString value = (val == eLow) ? "Low" : "High";
    qDebug() << __FUNCTION__ << " Pin=" << pin << " " << value;
#endif
}


int WiringPi::softPwmCreate(int pin, int initVal, int pwmRange)
{
#ifdef RASPBERRY_PI
      return ::softPwmCreate(pin, initVal, pwmRange);
#else
    qDebug() << __FUNCTION__ << " pin=" << pin << " initVal=" << initVal << " pwmRange=" << pwmRange;
    return 0;
#endif
}


void WiringPi::softPwmWrite(int pin, int value)
{
#ifdef RASPBERRY_PI
    ::softPwmWrite(pin, value);
#else
    qDebug() << __FUNCTION__ << " pin=" << pin << " value=" << value;
#endif
}


void WiringPi::pwmSetClock(int clock)
{
#ifdef RASPBERRY_PI
    ::pwmSetClock(clock);
#else
    qDebug() << __FUNCTION__ << " clock=" << clock;
#endif
}


void WiringPi::pwmSetRange(int range)
{
#ifdef RASPBERRY_PI
    ::pwmSetRange(range);
#else
    qDebug() << __FUNCTION__ << " range=" << range;
#endif
}


void WiringPi::pwmSetMode(PwmMode pwmMode)
{
#ifdef RASPBERRY_PI
    ::pwmSetMode(pwmMode == ePwmModeMs ? 0 : 1);
#else
    qDebug() << __FUNCTION__ << " pwmMode=" << pwmMode;
#endif
}
