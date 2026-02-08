#ifndef ADDABOARD_H
#define ADDABOARD_H

#include <QObject>
#include <plugins/plugincore/plugin.h>
#include <tagsystem/tag.h>

namespace plugin {

class AddaBoard : public Plugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "june.plugin.addaboard")
  public:
	AddaBoard() = default;

	bool initialize() final;

  protected slots:
	void mainloop() final;

  private:
	Tag *adc0Tag_ = nullptr;
	Tag *adc1Tag_ = nullptr;
	Tag *adc2Tag_ = nullptr;
	Tag *adc3Tag_ = nullptr;
	Tag *adc4Tag_ = nullptr;
	Tag *adc5Tag_ = nullptr;
	Tag *adc6Tag_ = nullptr;
	Tag *adc7Tag_ = nullptr;
};

} // namespace plugin

#endif
