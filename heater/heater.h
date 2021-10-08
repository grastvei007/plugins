#ifndef HEATER_H
#define HEATER_H

#include <plugins/pluginload/plugininterface.h>

#include <tagsystem/tag.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>

#include <QObject>
#include <QTimer>

#include "motor.h"
#include "preheatunit.h"
#include "pump.h"

class Heater:  public QObject, public PluginInterface
{
    Q_OBJECT
public:
    enum States
    {
        eOff,
        ePreHeat,
        eStarting,
        eRunning,
        eStoping
    };
    Heater();

    void setTagSystem(TagList *taglist) override;
    bool initialize() override;
    void run(int deltaMs) override;
    void stop() override;

private slots:
    void onPowerOnValueChanged(bool value);
    void onHeatLevelValueChanges(int value);

    void mainloop();

private:
    QString stateToString(States state);

    void stateOff();
    void statePreHeat();
    void stateStarting();
    void stateRunning();
    void stateStopping();

    QTimer* mainLoopTimer_ = nullptr;
    TagList *tagList_ = nullptr;
    States state_ = eOff;
    int heatLevel_ = 0;
    bool powerOn_ = false;

    Tag *powerOnTag_ = nullptr;
    Tag *heatLevelTag_ = nullptr;
    Tag *stateTag_ = nullptr;

    TagSocket *powerOnTagSocket_ = nullptr;
    TagSocket *heatLevelTagSocket_ = nullptr;

    Pump pump_;
    Motor MotorFan_;
    Motor MotorHeat_;
    PreHeatUnit preHeatUnit_;
};

#endif // HEATER_H
