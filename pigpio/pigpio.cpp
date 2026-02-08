#include "pigpio.h"
#include "pin.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <plugins/plugincore/path.h>
#include <tagsystem/taglist.h>

#include <algorithm>

namespace plugin{

bool PiGpio::initialize()
{
    WiringPi::setup();
    readConfigFile(configFileName_);
    return true;
}

void PiGpio::createApi(QHttpServer &httpserver)
{
    piGpioApi_.setupApi(httpserver);
}

void PiGpio::updateEnable(int wiringPin, bool enable)
{
    auto pin = std::ranges::find_if(pins_, [&wiringPin](const auto &pin) {
        return pin->wiringPiPin() == wiringPin;
    });

    if (pin == pins_.end())
        return;

    (*pin)->setEnable(enable);
}

void PiGpio::updateDirection(int wiringPin, WiringPi::PinDir dir)
{
    auto pin = std::ranges::find_if(pins_, [&wiringPin](const auto &pin) {
        return pin->wiringPiPin() == wiringPin;
    });

    if (pin == pins_.end())
        return;

    (*pin)->setDirection(dir);
}

void PiGpio::updateTagSocketHookUpTag(int wiringPin, const QString &tagFullName)
{
    auto pin = std::ranges::find_if(pins_, [&wiringPin](const auto &pin) {
        return pin->wiringPiPin() == wiringPin;
    });

    if (pin == pins_.end())
        return;

    (*pin)->hookupTagSocket(tagFullName);
}

QJsonArray PiGpio::toJson() const
{
    QJsonArray array;

    for (const auto &pin : pins_)
    {
        array.push_back(pin->toJson());
    }

    return array;
}

QJsonObject PiGpio::pinToJson(int wiringPiPin) const
{
    auto pin = std::ranges::find_if(pins_, [&wiringPiPin](const auto &pin) {
        return pin->wiringPiPin() == wiringPiPin;
    });
    if (pin == pins_.end())
        return QJsonObject();

    return (*pin)->toJson();
}

void PiGpio::loop() {}

void PiGpio::readConfigFile(const QString &configFile)
{
    auto filePath = path::fromConfigDir(configFile);
    qDebug() << filePath;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        file.close();
        return;
    }
    auto data = file.readAll();
    file.close();

    QJsonParseError *error = nullptr;
    QJsonDocument config = QJsonDocument::fromJson(data, error);
    if (error) {
        qDebug() << error->errorString();
        return;
    }
    QJsonObject gpio = config.object().value("gpio").toObject();
    const QJsonArray pins = gpio.value("pins").toArray();

    for (const auto &pinRef : pins)
    {
        auto pin = pinRef.toObject();

        // create an enabled pin object
        auto enabled = pin.value("enabled").toBool();
        auto subsystem = pin.value("subsystem").toString();
        auto name = pin.value("tag").toString();
        auto description = pin.value("description").toString();
        int wiringPiPin = pin.value("wiringpi").toInt();
        auto dirStr = pin.value("dir").toString();
        auto dir =  dirToEnum(dirStr);

        if(!dir.has_value())
            continue;

        auto *tag = tagList()->createTag(subsystem, QString("%1_%2").arg(name, dirStr) , TagType::eInt, 0, description);
        auto *tagsocket = TagSocket::createTagSocket(subsystem, name, TagSocket::eInt);
        Pin *p = new Pin(tagsocket, tag, wiringPiPin, dir.value());
        p->setEnable(enabled);
        pins_.emplace_back(std::move(p));
    }

}

std::optional<WiringPi::PinDir> PiGpio::dirToEnum(const QString &str)
{
    if(str.contains("out"))
        return WiringPi::PinDir::eOutput;
    else if(str.contains("in"))
        return WiringPi::PinDir::eInput;
    else if(str.contains("pwm"))
        return WiringPi::PinDir::ePwm;
    return std::nullopt;
}

}// end namespace
