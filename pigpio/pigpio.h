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
#include "pigpioapi.h"

class QJsonArray;
class QJsonObject;

namespace plugin{


/* Developed for rasperry pi 3B */
class PiGpio : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.pigpio")
public:
    PiGpio() = default;

    bool initialize() final;
    void createApi(QHttpServer &httpserver) final;

    void updateEnable(int wiringPin, bool enable);
    void updateDirection(int wiringPi, WiringPi::PinDir dir);
    void updateTagSocketHookUpTag(int wiringPin, const QString& tagFullName);

    QJsonArray toJson() const;
    QJsonObject pinToJson(int wiringPiPin) const;

    std::optional<WiringPi::PinDir> dirToEnum(const QString &str);
private slots:
    void mainloop() final;

private:
    const QString configFileName_{"pigpio.json"};
    void readConfigFile(const QString &configFile);
    std::vector<std::unique_ptr<Pin>> pins_;
    PiGpioApi piGpioApi_{*this};
};

} // end maespace plugin
#endif // PIGPIO_H
