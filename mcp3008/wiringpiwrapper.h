#ifndef WIRINGPIWRAPPER_H
#define WIRINGPIWRAPPER_H

#include <functional>

/**
 * Wrapper for wiringPi functions.
 * This is to prevent mistakes when develop.
 * it gives output for debuging, and can then compile
 * and run on the pi.
 */

class WiringPi
{
public:
    enum PinDir{
        eInput,
        eOutput
    };

    enum Value
    {
        eLow,
        eHigh
    };

    enum PwmMode
    {
        ePwmModeMs,
        ePwmModeBal
    };

    enum TriggerEdge
    {
        eINT_EDGE_FALLING,
        eINT_EDGE_RISING,
        eINT_EDGE_BOTH
    };

    static void setup();
    static void pinMode(int aPin, PinDir aDir);
    static void digitalWrite(int aPin, Value aVal);
    static int digitalRead(int pin);
    static int softPwmCreate(int aPin, int aInitVal, int aPwmRange);
    static void softPwmWrite(int aPin, int aValue);
    static void pwmSetClock(int aClock);
    static void pwmSetRange(int aRange);
    static void pwmSetMode(PwmMode aPwmMode);
    static int wiringPiISR (int pin, int edgeType, void (*function)(void));


    //mcp3008
    static void mcp3008Setup(int lowestPin, int spiChan);
    static int analogRead(int channel);
};




#endif // WIRINGPIWRAPPER_H
