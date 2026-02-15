#ifndef PLUGIN_VICTRONENERGY_H
#define PLUGIN_VICTRONENERGY_H

#include <plugins/plugincore/plugin.h>
#include <plugins/pluginload/plugininterface.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/tag.h>

#include <QObject>
#include <memory>
#include <vector>

#include "battery.h"

namespace plugin{

class VictronEnergy : public Plugin
{
    Q_OBJECT
    //
    Q_PLUGIN_METADATA(IID "june.plugin.victronenergy")
public:
    VictronEnergy(QObject *parent = nullptr) : Plugin(){}

    bool initialize() final;

protected slots:
    void mainloop() final;

private slots:
  void updateDaylyChaged();
  void updateDaylyDischarged();

private:
  void updateDailyEnergyUse();
  void resetValues();

  std::shared_ptr<Tag> victronTotalChargedTodayTag_;
  std::shared_ptr<Tag> victronTotalDiscargedTodayTag_;
  std::shared_ptr<Tag> victronTotalEneryUseToday_;

  std::vector<std::unique_ptr<Battery>> batteries_;

  int currentDay_ = -1;
};

}

#endif // VICTRONENERGY_H
