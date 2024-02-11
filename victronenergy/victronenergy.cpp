#include "victronenergy.h"

#include <tagsystem/taglist.h>

#include <QDate>

extern "C" PluginInterface* createPlugin()
{
    return new plugin::VictronEnergy();
}

namespace plugin {

bool VictronEnergy::initialize()
{
    // create tags
    victronTotalChargedTodayTag_.reset(tagList()->createTag("victron", "chargedToday", Tag::eInt, "dayily yield"));
    victronTotalDiscargedTodayTag_.reset(tagList()->createTag("victron", "dischargedToday", Tag::eInt, "dayly useage"));

    battery1ChargedTodayTag_.reset(tagList()->createTag("victron", "Battery1_ChargedToday", Tag::eInt, "dayly yield"));
    battery1DischargedTodayTag_.reset(tagList()->createTag("victron","Battery1_DischargedToday", Tag::eInt, "dayly usage"));

    battery2ChargedTodayTag_.reset(tagList()->createTag("victron", "Battery2_ChargedToday", Tag::eInt, "dayly yield"));
    battery2DischargedTodayTag_.reset(tagList()->createTag("victron", "Battery2_DischargedToday", Tag::eInt, "dayly usage"));

    // listen to values
    battery1ChargedEnergyTagSocket_.reset(TagSocket::createTagSocket("victron", "battery1Charged", TagSocket::eDouble));
    battery1DischargedEnergyTagSocket_.reset(TagSocket::createTagSocket("victron", "battery1Discharged", TagSocket::eDouble));
    battery2ChargedEnergyTagSocket_.reset(TagSocket::createTagSocket("victron", "battery2Charged", TagSocket::eDouble));
    battery2DischargedEnergyTagSocket_.reset(TagSocket::createTagSocket("victron", "battery2Discharged", TagSocket::eDouble));


    connect(battery1ChargedEnergyTagSocket_.get(), qOverload<double>(&TagSocket::valueChanged), this, &VictronEnergy::onBattery1ChargedEnergyChanged);
    connect(battery1DischargedEnergyTagSocket_.get(), qOverload<double>(&TagSocket::valueChanged), this, &VictronEnergy::onBattery1DischargedEnergyChanged);
    connect(battery2ChargedEnergyTagSocket_.get(), qOverload<double>(&TagSocket::valueChanged), this, &VictronEnergy::onBattery2ChargedEnergyChanged);
    connect(battery2DischargedEnergyTagSocket_.get(), qOverload<double>(&TagSocket::valueChanged), this, &VictronEnergy::onBattery2DischargedEnergyChanged);

    battery1ChargedEnergyTagSocket_->hookupTag("BMV700", "H18");
    battery1DischargedEnergyTagSocket_->hookupTag("BMV700", "H17");

    battery2ChargedEnergyTagSocket_->hookupTag("BMV702", "H18");
    battery2DischargedEnergyTagSocket_->hookupTag("BMV702", "H17");

    currentDay_ = QDate::currentDate().day();

    return true;
}

void VictronEnergy::mainloop()
{
    auto currentDay = QDate::currentDate().day();

    if(currentDay == currentDay_)
        return;

    // force update on next value
    battery1TotalChargedEnergy_ = -1;
    battery1TotalDischargedEnergy_ = -1;
    battery2TotalChargedEnergy_ = -1;
    battery2TotalDischargedEnergy_ = -1;

    // reset dayly values
    battery1ChargedEnergy_ = 0;
    battery1DischargedEnergy_ = 0;
    battery2ChargedEnergy_ = 0;
    battery2DischargedEnergy_ = 0;

    currentDay_ = currentDay;

}

void VictronEnergy::onBattery1ChargedEnergyChanged(double value)
{
    if(battery1TotalChargedEnergy_ < 0)
        battery1TotalChargedEnergy_ = value;

    battery1ChargedEnergy_ = value - battery1TotalChargedEnergy_;
    updateDaylyChaged();
}

void VictronEnergy::onBattery1DischargedEnergyChanged(double value)
{
    if(battery1TotalDischargedEnergy_ < 0)
        battery1TotalDischargedEnergy_ = value;

    battery1DischargedEnergy_ = value - battery1TotalDischargedEnergy_;
    updateDaylyDischarged();
}

void VictronEnergy::onBattery2ChargedEnergyChanged(double value)
{
    if(battery2TotalChargedEnergy_ < 0)
        battery2TotalChargedEnergy_ = value;

    battery2ChargedEnergy_ = value - battery2TotalChargedEnergy_;
    updateDaylyChaged();
}

void VictronEnergy::onBattery2DischargedEnergyChanged(double value)
{
    if(battery2TotalDischargedEnergy_ < 0)
        battery2TotalDischargedEnergy_ = value;

    battery2DischargedEnergy_ = value - battery2TotalDischargedEnergy_;
    updateDaylyDischarged();
}

void VictronEnergy::updateDaylyChaged()
{
    victronTotalChargedTodayTag_->setValue(battery1ChargedEnergy_ + battery2ChargedEnergy_);
    battery1ChargedTodayTag_->setValue(battery1ChargedEnergy_);
    battery2ChargedTodayTag_->setValue(battery2ChargedEnergy_);
}


void VictronEnergy::updateDaylyDischarged()
{
    victronTotalDiscargedTodayTag_->setValue(battery1DischargedEnergy_ + battery2DischargedEnergy_);
    battery1DischargedTodayTag_->setValue(battery1DischargedEnergy_);
    battery2DischargedTodayTag_->setValue(battery2DischargedEnergy_);
}

}//end namespace
