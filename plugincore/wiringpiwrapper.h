#ifndef WIRINGPIWRAPPER2_H
#define WIRINGPIWRAPPER2_H

/**
 * Wrapper for wiringPi functions.
 * This is to prevent mistakes when develop.
 * it gives output for debuging, and can then compile
 * and run on the pi.
 */

namespace WiringPi {

//public:
enum PinDir { eInput, eOutput };

enum Value { eLow, eHigh };

enum PwmMode { ePwmModeMs, ePwmModeBal };

enum TriggerEdge { eINT_EDGE_FALLING, eINT_EDGE_RISING, eINT_EDGE_BOTH };

void setup();
void pinMode(int aPin, PinDir aDir);
void digitalWrite(int aPin, Value aVal);
int digitalRead(int pin);
int softPwmCreate(int aPin, int aInitVal, int aPwmRange);
void softPwmWrite(int aPin, int aValue);
void pwmSetClock(int aClock);
void pwmSetRange(int aRange);
void pwmSetMode(PwmMode aPwmMode);
int wiringPiISR(int pin, int edgeType, void (*function)(void));

void delay(unsigned int ms);

// SPI
void wiringPiSPISetupMode(int spiChannel, int speed, int mode);
int wiringPiSPIDataRW(int spiChannel, unsigned char *buffer, int size);

} // namespace WiringPi

#endif // WIRINGPIWRAPPER_H
