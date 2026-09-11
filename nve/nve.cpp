#include "nve.h"
#include <tagsystem/util/date.h>
#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <QTime>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace plugin
{

bool Nve::initialize()
{
	no1_ = tagList()->createTag(subSystem(), "no1", TagType::eDouble, false);
	no2_ = tagList()->createTag(subSystem(), "no2", TagType::eDouble, false);
	no3_ = tagList()->createTag(subSystem(), "no3", TagType::eDouble, false);
	no4_ = tagList()->createTag(subSystem(), "no4", TagType::eDouble, false);
	no5_ = tagList()->createTag(subSystem(), "no5", TagType::eDouble, false);


	connect(&nas_, &QNetworkAccessManager::finished, this, &Nve::onResponse);
	return true;
}

void Nve::mainloop()
{
	switch (util::date::currentDay()) {
		case util::date::DayOfWeek::eWednesDay:
			{
				auto time = QTime::currentTime();
				if(time.hour() >= 14 && !hasSent_)
				{
					QHttpHeaders header;
					header.append(QHttpHeaders::WellKnownHeader::Accept, "application/json");
					QNetworkRequest request(QUrl("https://biapi.nve.no/magasinstatistikk/api/Magasinstatistikk/HentOffentligDataSisteUke"));
					request.setHeaders(header);
					nas_.get(request);
					hasSent_ = true;
				}
				break;
			}
		default:
			{
				hasSent_ = false;
				break;
			}
	}
}

void Nve::onResponse(QNetworkReply *response)
{
	auto jsonDocument = response->readAll();
	const auto &jsonArray = QJsonDocument::fromJson(jsonDocument).array();
	for(const auto &ref : jsonArray)
	{
		const auto &obj = ref.toObject();
		if(obj.contains("omrType") && obj.value("omrType").toString() == "EL")
		{
			int areaCode = obj.value("omrnr").toInt();
			double percent = obj.value("fyllingsgrad").toDouble() * 100;
			if(areaCode == 1)
			{
				no1_->setValue(percent);
			}
			else if(areaCode == 2)
			{
				no2_->setValue(percent);
			}
			else if(areaCode == 3)
			{
				no3_->setValue(percent);
			}
			else if(areaCode == 4)
			{
				no4_->setValue(percent);
			}
			else if(areaCode == 5)
			{
				no5_->setValue(percent);
			}
		}
	}
}

} // end namespace