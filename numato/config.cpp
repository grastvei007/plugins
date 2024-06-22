#include "numato.h"
#include <QString>
#include <QDir>
#include <QFile>

#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

#include <optional>

#include <tagsystem/taglist.h>

namespace plugin{

std::optional<Gpio> intToGpio(int gpio)
{
    if(gpio == 0)
        return Gpio::Gpio_0;
    else if(gpio == 1)
        return Gpio::Gpio_1;
    else if(gpio == 2)
        return Gpio::Gpio_2;
    else if(gpio == 3)
        return Gpio::Gpio_3;
    else if(gpio == 4)
        return Gpio::Gpio_4;
    else if(gpio == 5)
        return Gpio::Gpio_5;
    else if(gpio == 6)
        return Gpio::Gpio_6;
    else if(gpio == 7)
        return Gpio::Gpio_7;
    else if(gpio == 8)
        return Gpio::Gpio_8;
    else if(gpio == 9)
        return Gpio::Gpio_9;
    else if(gpio == 10)
        return Gpio::Gpio_10;
    else if(gpio == 11)
        return Gpio::Gpio_11;
    else if(gpio == 12)
        return Gpio::Gpio_12;
    else if(gpio == 13)
        return Gpio::Gpio_13;
    else if(gpio == 14)
        return Gpio::Gpio_14;
    else if(gpio == 15)
        return Gpio::Gpio_15;

    return {};
}


void Numato::loadSettings()
{
#ifdef __linux__
    QString path = QDir::homePath() + QDir::separator() + ".config" + QDir::separator() + "june";
#else
    QString path = qApp->applicationDirPath();
#endif
    path.append(QDir::separator() + configFile_);

    QFile file(path);
    if(!file.exists())
    {
        qDebug() << __FUNCTION__ << "File does not exist: " << path;
        return;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ << "Unable to open file: " << path;
        qDebug() << file.errorString();
        return;
    }

    QJsonParseError parseError;
    QByteArray data = file.readAll();

    QJsonObject obj = QJsonDocument::fromJson(data, &parseError).object();
    if(parseError.error != QJsonParseError::NoError)
        qDebug() << parseError.errorString();

    QJsonArray devicesArray = obj["devices"].toArray();

    for(auto device : devicesArray)
    {
        auto jsonDevice = device.toObject();
        deviceAdress_ = jsonDevice["ip"].toString();
        deviceName_ = jsonDevice["name"].toString();
        loginUserName_ = jsonDevice["user"].toString();
        loginPassword_ = jsonDevice["password"].toString();

        QJsonArray gpio = jsonDevice["gpio"].toArray();
        for(auto pinRef : gpio)
        {
            auto pin = pinRef.toObject();
            int pinInt = pin["gpio"].toInt();
            auto pinId = intToGpio(pinInt);

            if(pinId.has_value())
            {
                QString dir = pin["iodir"].toString();
                bool defaultValue = pin["value"].toString() == "off" ? false : true;
                initGpioPin(pinId.value(), dir, defaultValue);
            }
        }

    }
}

void Numato::initGpioPin(Gpio gpio, QString iodir, bool value)
{
    Q_UNUSED(value); // skipped for now, maybe used in the future
    bool dir = iodir == "out" ? false : true;

    iodir_.set(gpioNumberToInt(gpio), dir);
    iomask_.set(gpioNumberToInt(gpio), true);

    if(dir)
    {
        auto tag = tagList()->createTag(deviceName_, QString("gpio_%1").arg(gpioToString(gpio)), Tag::eBool, 0, "Gpio input");
        gpioInput_[gpio] = tag;
    }
    else
    {
        auto tag = tagList()->createTag(deviceName_, gpioToString(gpio), Tag::eBool, 0, "Gpio output");
        auto *tagSocket = TagSocket::createTagSocket(deviceName_, QString("gpio_%1").arg(gpioToString(gpio)), TagSocket::eBool);
        tagSocket->hookupTag(tag);

        // Update device when tagsocket change
        connect(tagSocket, qOverload<bool>(&TagSocket::valueChanged), this, [this, gpio](bool value){
            if(value)
                set(gpio);
            else
                clear(gpio);
        });

        gpioOutput_[gpio] = std::unique_ptr<TagSocket>(std::move(tagSocket));
    }
}


} // end namespace
