#ifndef PLUGIN_VICTRONENERGY_H
#define PLUGIN_VICTRONENERGY_H

#include <plugins/plugincore/plugin.h>
#include <plugins/pluginload/plugininterface.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/tag.h>

#include <QObject>
#include <memory>

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
    void onBattery1ChargedEnergyChanged(double value);
    void onBattery1DischargedEnergyChanged(double value);
    void onBattery2ChargedEnergyChanged(double value);
    void onBattery2DischargedEnergyChanged(double value);

private:
    void updateDaylyChaged();
    void updateDaylyDischarged();
    void resetValues();

    std::shared_ptr<TagSocket> battery1ChargedEnergyTagSocket_;
    std::shared_ptr<TagSocket> battery1DischargedEnergyTagSocket_;
    std::shared_ptr<TagSocket> battery2ChargedEnergyTagSocket_;
    std::shared_ptr<TagSocket> battery2DischargedEnergyTagSocket_;

    std::shared_ptr<Tag> victronTotalChargedTodayTag_;
    std::shared_ptr<Tag> victronTotalDiscargedTodayTag_;

    std::shared_ptr<Tag> battery1ChargedTodayTag_;
    std::shared_ptr<Tag> battery1DischargedTodayTag_;

    std::shared_ptr<Tag> battery2ChargedTodayTag_;
    std::shared_ptr<Tag> battery2DischargedTodayTag_;


    // total updated ot midnight
    int battery1TotalChargedEnergy_ = -1;
    int battery1TotalDischargedEnergy_ = -1;
    int battery2TotalChargedEnergy_ = -1;
    int battery2TotalDischargedEnergy_ = -1;

    // dayly
    int battery1ChargedEnergy_ = 0;
    int battery1DischargedEnergy_ = 0;
    int battery2ChargedEnergy_ = 0;
    int battery2DischargedEnergy_ = 0;

    int currentDay_ = -1;
};

}

#endif // VICTRONENERGY_H
