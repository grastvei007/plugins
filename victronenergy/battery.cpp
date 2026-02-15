#include "battery.h"

namespace plugin {

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

	amphereTagSocket_.reset(TagSocket::createTagSocket(subsystem, name + "_I,", TagSocket::eDouble));
	powerTagSocket_.reset(TagSocket::createTagSocket(subsystem, name + "_P", TagSocket::eInt));

	connect(chargedEnergyTagSocket_.get(),
			qOverload<double>(&TagSocket::valueChanged),
			this,
			&Battery::onChargedEnergyChanged);

	connect(dischargedEnergyTagSocket_.get(),
			qOverload<double>(&TagSocket::valueChanged),
			this,
			&Battery::onDischargedEnergyChanged);

	connect(amphereTagSocket_.get(),
			qOverload<double>(&TagSocket::valueChanged),
			this,
			&Battery::onAmphereChanged);

	connect(powerTagSocket_.get(),
			qOverload<int>(&TagSocket::valueChanged),
			this,
			&Battery::onPowerChanged);

	chargedEnergyTagSocket_->hookupTag(batteyName, "H18");
	dischargedEnergyTagSocket_->hookupTag(batteyName, "H17");
	amphereTagSocket_->hookupTag(batteyName, "I");
	powerTagSocket_->hookupTag(batteyName, "P");
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

void Battery::onAmphereChanged(double value)
{
	amphere_ = value;
	emit amphereChanged();
}

void Battery::onPowerChanged(int value)
{
	power_ = value;
	emit powerChanged();
}

} // namespace plugin
