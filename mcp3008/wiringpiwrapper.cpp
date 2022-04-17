#include "wiringpiwrapper.h"

#ifdef RASPBERRY_PI
#include <wiringPi.h>
#include <mcp3004.h>
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

int WiringPi::digitalRead(int pin)
{
#ifdef RASPBERRY_PI
        return ::digitalRead(pin);
#else
    qDebug() << __FUNCTION__;
    return 0;
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

int WiringPi::wiringPiISR(int pin, int edgeType, void (*function)())
{
#ifdef RASPBERRY_PI
    if(edgeType == eINT_EDGE_FALLING)
        return ::wiringPiISR (pin, INT_EDGE_FALLING,  function);
    else if( edgeType == eINT_EDGE_RISING)
        return ::wiringPiISR (pin, INT_EDGE_RISING,  function);
    else if(edgeType == eINT_EDGE_BOTH)
        return ::wiringPiISR (pin, INT_EDGE_BOTH,  function);
#else
    qDebug() << __FUNCTION__;
    return 0;
#endif
}

void WiringPi::mcp3008Setup(int lowestPin, int spiChan)
{
#ifdef RASPBERRY_PI
    ::mcp3004Setup(lowestPin, spiChan);
#endif
}

int WiringPi::analogRead(int channel)
{
#ifdef RASPBERRY_PI
    return ::analogRead(channel);
#endif

    return 0;
}
