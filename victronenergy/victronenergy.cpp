#include "victronenergy.h"

#include <tagsystem/taglist.h>
#include <tagsystem/util/time.h>

#include <QSettings>
#include <QTime>

namespace plugin {

bool VictronEnergy::initialize()
{
	const QString subsystem("victron");

	// create tags
	victronTotalChargedTodayTag_.reset(
		tagList()->createTag("victron", "chargedToday", TagType::eInt, "dayily yield"));
	victronTotalDiscargedTodayTag_.reset(
		tagList()->createTag("victron", "dischargedToday", TagType::eInt, "dayly useage"));
	victronTotalEneryUseToday_.reset(
		tagList()->createTag("victron", "energy_use_today", TagType::eInt, "daily ussage"));
	victronMpptsTotalYield_.reset(tagList()->createTag(subsystem,
													   "totoal_yield",
													   TagType::eDouble,
													   "dayily yield from mppts"));

	combineAmphereTag_.reset(tagList()->createTag(subsystem,
												  "combined_I",
												  TagType::eDouble,
												  "Total I from all batteries"));
	compinePowerTag_.reset(
		tagList()->createTag(subsystem, "combined_P", TagType::eInt, "Total P from all batteries"));

	QSettings settings("june", "june");
	settings.beginGroup("batteries");
	for (const auto &batteryConfig : settings.childKeys())
	{
		QString name = settings.value(batteryConfig).toString();

		batteries_.emplace_back(
			std::make_unique<Battery>(tagList(), subsystem, name, batteryConfig));

		connect(batteries_.back().get(),
				&Battery::chargedEnergyChanged,
				this,
				&VictronEnergy::updateDaylyChaged);
		connect(batteries_.back().get(),
				&Battery::dischargedEnergyChanged,
				this,
				&VictronEnergy::updateDaylyDischarged);
		connect(batteries_.back().get(),
				&Battery::amphereChanged,
				this,
				&VictronEnergy::onBatteryAmphereChanged);
		connect(batteries_.back().get(),
				&Battery::powerChanged,
				this,
				&VictronEnergy::onBatteryPowerChanged);
	}

	settings.beginGroup("mppt");
	for (const auto &mpptConfig : settings.childKeys())
	{
		QString serialNumber = settings.value(mpptConfig).toString();

		mppts_.emplace_back(std::make_unique<Mppt>(tagList(), serialNumber));
	}

	connect(tagList(), &TagList::initialTagBurst, this, &VictronEnergy::resetValues);

	return true;
}

void VictronEnergy::mainloop()
{
	auto currentTime = QTime::currentTime().msecsSinceStartOfDay();

	// only reset valued once a day at time 23:55
	if (!util::time::isInTimeSlot(23, 55, runTimeStep(), currentTime))
		return;

	resetValues();

	double sum = 0.0;
	for (auto &mppt : mppts_)
	{
		sum += mppt->yield();
		mppt->reset();
	}
	victronMpptsTotalYield_->setValue(sum);
}

void VictronEnergy::updateDaylyChaged()
{
	int sum = 0;
	for (const auto &battery : batteries_)
		sum += battery->chargedEnergy();

	victronTotalChargedTodayTag_->setValue(sum);
	updateDailyEnergyUse();
}


void VictronEnergy::updateDaylyDischarged()
{
	int sum = 0;
	for (const auto &battery : batteries_)
		sum += battery->dischargedEnergy();

	victronTotalDiscargedTodayTag_->setValue(sum);
	updateDailyEnergyUse();
}

void VictronEnergy::onBatteryAmphereChanged()
{
	double sum = 0.0;
	for (const auto &battery : batteries_)
	{
		sum += battery->amphere();
	}
	combineAmphereTag_->setValue(sum);
}

void VictronEnergy::onBatteryPowerChanged()
{
	int sum = 0;
	for (const auto &battery : batteries_)
	{
		sum += battery->power();
	}
	compinePowerTag_->setValue(sum);
}

void VictronEnergy::updateDailyEnergyUse()
{
	int sumDischarge = 0;
	int sumCharge = 0;
	for (const auto &battery : batteries_)
	{
		sumDischarge += battery->dischargedEnergy();
		sumCharge += battery->chargedEnergy();
	}
	victronTotalEneryUseToday_->setValue(sumCharge - sumDischarge);
}

void VictronEnergy::resetValues()
{
	// reset dayly values
	for (auto &battery : batteries_)
		battery->resetValues();

	updateDaylyChaged();
	updateDaylyDischarged();
}

}//end namespace
