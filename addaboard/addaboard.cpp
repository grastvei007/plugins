#include "addaboard.h"

#include "3rdparty/ADS1256.h"
#include "3rdparty/DEV_Config.h"

#include <tagsystem/taglist.h>

namespace plugin {

bool AddaBoard::initialize()
{
	qDebug() << __FUNCTION__;
	DEV_ModuleInit();

	const QString subsystem("adda_board");

	adc0Tag_ = tagList()->createTag(subsystem, "adc0", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc1Tag_ = tagList()->createTag(subsystem, "adc1", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc2Tag_ = tagList()->createTag(subsystem, "adc2", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc3Tag_ = tagList()->createTag(subsystem, "adc3", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc4Tag_ = tagList()->createTag(subsystem, "adc4", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc5Tag_ = tagList()->createTag(subsystem, "adc5", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc6Tag_ = tagList()->createTag(subsystem, "adc6", TagType::eDouble, 0.0, "Analog read [0-5]v");
	adc7Tag_ = tagList()->createTag(subsystem, "adc7", TagType::eDouble, 0.0, "Analog read [0-5]v");

	return true;
}

void AddaBoard::loop()
{
	qDebug() << __FUNCTION__;
	UDOUBLE adc[8];
	ADS1256_GetAll(adc);

	adc0Tag_->setValue(adc[0] * 5.0 / 0x7fffff);
	adc1Tag_->setValue(adc[1] * 5.0 / 0x7fffff);
	qDebug() << "1: " << adc[1] * 5.0 / 0x7fffff;
	adc2Tag_->setValue(adc[2] * 5.0 / 0x7fffff);
	adc3Tag_->setValue(adc[3] * 5.0 / 0x7fffff);
	adc4Tag_->setValue(adc[4] * 5.0 / 0x7fffff);
	adc5Tag_->setValue(adc[5] * 5.0 / 0x7fffff);
	adc6Tag_->setValue(adc[6] * 5.0 / 0x7fffff);
	adc7Tag_->setValue(adc[7] * 5.0 / 0x7fffff);
}

} // namespace plugin
