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
#include "button.h"

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
    void onHeatLevelValueChanged(int value);
    void onFanLevelValueChanged(int value);
    void onIsBurningValueChanged(bool value);

    void onButtonHeatDownPushed(int level);
    void onButtonHeatUpPushed(int level);
    void onButtonPower(int level);

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
    int fanLevel_ = 0;
    int currentHeatLevel_ = 0;
    int currentFanLevel_ = 0;
    bool powerOn_ = false;
    int deltaMs_ = 0;
    bool isBurning_ = false;

    Tag *powerOnTag_ = nullptr;
    Tag *heatLevelTag_ = nullptr;
    Tag *fanLevelTag_ = nullptr;
    Tag *stateTag_ = nullptr;
    Tag *isBurningTag_ = nullptr;

    TagSocket *powerOnTagSocket_ = nullptr;
    TagSocket *heatLevelTagSocket_ = nullptr;
    TagSocket *fanLevelTagSocket_ = nullptr;
    TagSocket *isBurningTagSocket_ = nullptr;

    Pump pump_ = Pump(1);
    Motor motorFan_ = Motor(3);
    Motor motorHeat_ = Motor(2);
    PreHeatUnit preHeatUnit_ = PreHeatUnit(0);

    Button buttonHeatUp_ = Button(29, WiringPi::eINT_EDGE_BOTH);
    Button buttonHeatDown_ = Button(28, WiringPi::eINT_EDGE_BOTH);
    Button buttonPower_ = Button(25, WiringPi::eINT_EDGE_BOTH);

    double preHeatTime_ = 0;
    double stoppingTime_ = 0;
};

#endif // HEATER_H
