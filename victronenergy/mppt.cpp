#include "mppt.h"

#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>

namespace plugin {

Mppt::Mppt(TagList *taglist, const QString &serialNumber)
{
	yieldTagSocket_.reset(TagSocket::createTagSocket("victron", serialNumber, TagSocket::eInt));

	connect(yieldTagSocket_.get(),
			qOverload<int>(&TagSocket::valueChanged),
			this,
			&Mppt::onYieldChanged);

	yieldTagSocket_->hookupTag(serialNumber, "H20");
}

void Mppt::reset()
{
	daylyYield_ = 0;
}

void Mppt::onYieldChanged(int value)
{
	if (value > daylyYield_)
		daylyYield_ = value;
}

} // namespace plugin
