#ifndef NVE_H
#define NVE_H

#include <plugins/plugincore/plugin.h>

#include <QObject>
#include <QNetworkAccessManager>

class Tag;

namespace plugin
{


class Nve : public Plugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "june.plugin.nve")
public:
	explicit Nve() : Plugin("nve"){};

  bool initialize() override;


private slots:
	void mainloop() final;
	void onResponse(QNetworkReply *response);
private:
	QNetworkAccessManager nas_;
	bool hasSent_ = false;

	Tag *no1_ = nullptr;
	Tag *no2_ = nullptr;
	Tag *no3_ = nullptr;
	Tag *no4_ = nullptr;
	Tag *no5_ = nullptr;
};

} // end namespace

#endif