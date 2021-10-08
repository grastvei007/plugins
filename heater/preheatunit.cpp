#include "preheatunit.h"

#include "wiringpiwrapper.h"

PreHeatUnit::PreHeatUnit(int wireingPiPin) : wireingPiPin_(wireingPiPin)
{
    WiringPi::pinMode(wireingPiPin_, WiringPi::eOutput);
}

void PreHeatUnit::setActive(bool active)
{
    isActive_ = active;
    if(isActive_)
        WiringPi::digitalWrite(wireingPiPin_, WiringPi::eHigh);
    else
        WiringPi::digitalWrite(wireingPiPin_, WiringPi::eLow);
}

bool PreHeatUnit::isActive() const
{
    return isActive_;
}
