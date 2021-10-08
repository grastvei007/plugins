#include "preheatunit.h"

PreHeatUnit::PreHeatUnit()
{

}

void PreHeatUnit::setActive(bool active)
{
    isActive_ = active;
}

bool PreHeatUnit::isActive() const
{
    return isActive_;
}
