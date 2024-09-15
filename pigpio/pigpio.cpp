#include "pigpio.h"
#include "wiringpiwrapper.h"
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
    readConfigFile("pigpio.json");
    return true;
}


void PiGpio::mainloop()
{

}

void PiGpio::readConfigFile(const QString &configFile)
{
    auto filePath = path::fromConfigDir(configFile);
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << file.errorString();
        return;
    }
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();

    QJsonParseError *error = nullptr;
    QJsonDocument config = QJsonDocument::fromJson(data, error);
    if (error) {
        qDebug() << error->errorString();
        return;
    }
    QJsonObject gpio = config.object();
    QJsonArray pins = gpio.value("pins").toArray();

    for (auto &pinRef : pins)
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
        auto dir = pin.value("dir").toString() == "out" ? WiringPi::PinDir::eOutput
                                                        : WiringPi::PinDir::eInput;

        auto *tag = tagList()->createTag(subsystem, QString("%1_%2").arg(name, dirStr) , Tag::eInt, 0, description);
        if(dir == WiringPi::PinDir::eOutput)
        {
            // only create tagsocket for output pins.
            auto *tagsocket = TagSocket::createTagSocket(subsystem, name, TagSocket::eInt);
            tagsocket->hookupTag(tag);
            pins_.emplace_back(new Pin(tagsocket, tag, wiringPiPin, dir));
        }
        else
            pins_.emplace_back(new Pin(nullptr, tag, wiringPiPin, dir));
    }

}

}// end namespace
