#include "victronenergy.h"

#include <tagsystem/taglist.h>

#include <QDate>
#include <QSettings>

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
	}

	connect(tagList(), &TagList::initialTagBurst, this, &VictronEnergy::resetValues);

	return true;
}

void VictronEnergy::mainloop()
{
    auto currentDay = QDate::currentDate().day();

    if(currentDay == currentDay_)
        return;

    currentDay_ = currentDay;
    resetValues();
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

Battery::Battery(TagList *tagList,
				 const QString &subsystem,
				 const QString &name,
				 const QString &batteyName,
				 QObject *parent)
	: QObject(parent)
{
	chargedTodayTag_.reset(
		tagList->createTag(subsystem, name + "_charged_today", TagType::eInt, "dayly yield"));
	dischargedTodayTag_.reset(
		tagList->createTag(subsystem, name + "_discharged_today", TagType::eInt, "dayly usage"));
	chargedEnergyTagSocket_.reset(
		TagSocket::createTagSocket(subsystem, name + "Charged", TagSocket::eDouble));
	dischargedEnergyTagSocket_.reset(
		TagSocket::createTagSocket(subsystem, name + "Discharged", TagSocket::eDouble));

	connect(chargedEnergyTagSocket_.get(),
			qOverload<double>(&TagSocket::valueChanged),
			this,
			&Battery::onChargedEnergyChanged);

	connect(dischargedEnergyTagSocket_.get(),
			qOverload<double>(&TagSocket::valueChanged),
			this,
			&Battery::onDischargedEnergyChanged);

	chargedEnergyTagSocket_->hookupTag(batteyName, "H18");
	dischargedEnergyTagSocket_->hookupTag(batteyName, "H17");
}

void Battery::resetValues()
{
	// total updated at midnight
	totalChargedEnergy_ = -1;
	totalDischargedEnergy_ = -1;
	// dayly
	chargedEnergy_ = 0;
	dischargedEnergy_ = 0;
}

void Battery::onChargedEnergyChanged(double value)
{
	if (totalChargedEnergy_ < 0)
		totalChargedEnergy_ = value;

	chargedEnergy_ = value - totalChargedEnergy_;
	chargedTodayTag_->setValue(chargedEnergy_);
	emit chargedEnergyChanged();
}

void Battery::onDischargedEnergyChanged(double value)
{
	if (totalDischargedEnergy_ < 0)
		totalDischargedEnergy_ = value;

	dischargedEnergy_ = value - totalDischargedEnergy_;
	dischargedTodayTag_->setValue(dischargedEnergy_);
	emit dischargedEnergyChanged();
}

}//end namespace
