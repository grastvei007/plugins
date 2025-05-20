#include "pigpio.h"
#include "pin.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <plugins/plugincore/path.h>
#include <tagsystem/taglist.h>

namespace plugin{

bool PiGpio::initialize()
{
    WiringPi::setup();
    readConfigFile("pigpio.json");
    return true;
}


void PiGpio::mainloop()
{

}

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
        if (!pin.value("enabled").toBool())
            continue;

        // create an enabled pin object
        auto subsystem = pin.value("subsystem").toString();
        auto name = pin.value("tag").toString();
        auto description = pin.value("description").toString();
        int wiringPiPin = pin.value("wiringpi").toInt();
        auto dirStr = pin.value("dir").toString();
        auto dir =  dirToEnum(dirStr);

        if(!dir.has_value())
            continue;

        auto *tag = tagList()->createTag(subsystem, QString("%1_%2").arg(name, dirStr) , Tag::eInt, 0, description);
        if(dir.value() == WiringPi::PinDir::eOutput || dir.value() == WiringPi::PinDir::ePwm)
        {
            // only create tagsocket for output pins.
            auto *tagsocket = TagSocket::createTagSocket(subsystem, name, TagSocket::eInt);
            tagsocket->hookupTag(tag);
            pins_.emplace_back(new Pin(tagsocket, tag, wiringPiPin, dir.value()));
        }
        else
            pins_.emplace_back(new Pin(nullptr, tag, wiringPiPin, dir.value()));
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
