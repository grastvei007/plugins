#include "mppt.h"

#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>

namespace plugin {

Mppt::Mppt(TagList *taglist, const QString &serialNumber)
{
	yieldTagSocket_.reset(TagSocket::createTagSocket("victron", serialNumber + "_yield", TagSocket::eDouble));
	stateOfOperationTagSocket_.reset(TagSocket::createTagSocket("victron", serialNumber + "_cs", TagSocket::eInt));

	connect(yieldTagSocket_.get(),
			qOverload<double>(&TagSocket::valueChanged),
			this,
			&Mppt::onYieldChanged);

	connect(stateOfOperationTagSocket_.get(), qOverload<int>(&TagSocket::valueChanged), this, &Mppt::onStateOfOperationChanged);

	yieldTagSocket_->hookupTag(serialNumber, "H20");
	stateOfOperationTagSocket_->hookupTag(serialNumber, "CS");
}

void Mppt::reset()
{
	daylyYield_ = 0;
	yield_.bulk_ = 0.0;
	yield_.absorption_ = 0.0;
	yield_.float_ = 0.0;
}

void Mppt::onYieldChanged(double value)
{
	double deltaYield = value - daylyYield_;

	if (value > daylyYield_)
		daylyYield_ = value;

	if(stateOfOperation_ == global::victron::CS::Bulk)
	{
		yield_.bulk_ += deltaYield;
	}
	else if(stateOfOperation_ == global::victron::CS::Absorption)
	{
		yield_.absorption_ += deltaYield;
	}
	else if(stateOfOperation_ == global::victron::CS::Float)
	{
		yield_.float_ += deltaYield;
	}
}

void Mppt::onStateOfOperationChanged(int state)
{
	stateOfOperation_ = static_cast<global::victron::CS>(state);
}

} // namespace plugin
