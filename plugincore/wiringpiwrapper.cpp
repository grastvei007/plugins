#include "wiringpiwrapper.h"

#ifdef RASPBERRY_PI
#include <iostream>
#include <softPwm.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#else
#include <QDebug>
#include <QString>
#endif

namespace WiringPi {

void setup()
{
#ifdef RASPBERRY_PI
	std::cerr << __FUNCTION__ << std::endl;
    wiringPiSetup();
#else
    qDebug() << __FUNCTION__;
#endif
}

void pinMode(int pin, PinDir dir)
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

void digitalWrite(int pin, Value val)
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

int digitalRead(int pin)
{
#ifdef RASPBERRY_PI
        return ::digitalRead(pin);
#else
    qDebug() << __FUNCTION__;
    return 0;
#endif
}

int softPwmCreate(int pin, int initVal, int pwmRange)
{
#ifdef RASPBERRY_PI
      return ::softPwmCreate(pin, initVal, pwmRange);
#else
    qDebug() << __FUNCTION__ << " pin=" << pin << " initVal=" << initVal << " pwmRange=" << pwmRange;
    return 0;
#endif
}

void softPwmWrite(int pin, int value)
{
#ifdef RASPBERRY_PI
    ::softPwmWrite(pin, value);
#else
    qDebug() << __FUNCTION__ << " pin=" << pin << " value=" << value;
#endif
}

void pwmSetClock(int clock)
{
#ifdef RASPBERRY_PI
    ::pwmSetClock(clock);
#else
    qDebug() << __FUNCTION__ << " clock=" << clock;
#endif
}

void pwmSetRange(int range)
{
#ifdef RASPBERRY_PI
    ::pwmSetRange(range);
#else
    qDebug() << __FUNCTION__ << " range=" << range;
#endif
}

void pwmSetMode(PwmMode pwmMode)
{
#ifdef RASPBERRY_PI
    ::pwmSetMode(pwmMode == ePwmModeMs ? 0 : 1);
#else
    qDebug() << __FUNCTION__ << " pwmMode=" << pwmMode;
#endif
}

int wiringPiISR(int pin, int edgeType, void (*function)())
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

void wiringPiSPISetupMode(int spiChannel, int speed, int mode)
{
#ifdef RASPBERRY_PI
	::wiringPiSPISetupMode(spiChannel, speed, mode);
#else
	qDebug() << __FUNCTION__;
#endif
}

int wiringPiSPIDataRW(int spiChannel, unsigned char *buffer, int size)
{
#ifdef RASPBERRY_PI
	return ::wiringPiSPIDataRW(spiChannel, buffer, size);
#else
	qDebug() << __FUNCTION__;
	return 0;
#endif
}

void delay(unsigned int ms)
{
#ifdef RASPBERRY_PI
	::delay(ms);
#endif
}

} // namespace WiringPi
