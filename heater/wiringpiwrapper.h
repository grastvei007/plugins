#ifndef WIRINGPIWRAPPER_H
#define WIRINGPIWRAPPER_H

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

    static void wiringPiSetup();
    static void pinMode(int aPin, PinDir aDir);
    static void digitalWrite(int aPin, Value aVal);
    static int softPwmCreate(int aPin, int aInitVal, int aPwmRange);
    static void softPwmWrite(int aPin, int aValue);
    static void pwmSetClock(int aClock);
    static void pwmSetRange(int aRange);
    static void pwmSetMode(PwmMode aPwmMode);

};




#endif // WIRINGPIWRAPPER_H
