#ifndef PIGPIO_H
#define PIGPIO_H

#include <plugins/plugincore/plugin.h>
#include "wiringpiwrapper.h"
#include <QObject>
#include <QTimer>
#include <vector>

#include <memory>
#include <optional>

#include "pin.h"

namespace plugin{


/* Developed for rasperry pi 3B */
class PiGpio : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.pigpio")
public:
    PiGpio() = default;

    bool initialize() final;

private slots:
    void mainloop() final;

private:
    void readConfigFile(const QString &configFile);
    std::optional<WiringPi::PinDir> dirToEnum(const QString &str);
    std::vector<std::unique_ptr<Pin>> pins_;
};

} // end maespace plugin
#endif // PIGPIO_H
