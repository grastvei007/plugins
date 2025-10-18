#include "ds18b20.h"

#include <QTimer>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QDebug>

#include <tagsystem/taglist.h>

namespace plugin {

bool Ds18b20::initialize()
{
    folderToName_.emplace("28-01203c3bc31a", "indoor");
    folderToName_.emplace("28-01203c27b0a0", "outside");
    folderToName_.emplace("28-01203c2842ab", "dirt_temperature");

    const QString path = "/sys/bus/w1/devices";
    QDir dir(path);
    const QStringList dirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

    for(const QString &folder : dirs)
    {
        if(!folder.startsWith("28-"))
            continue;
        QString str(path);
        str.append(QDir::separator());
        str.append(folder);
        str.append(QDir::separator());
        str.append("w1_slave");
        auto name = folder;
        if(folderToName_.count(folder))
            name = folderToName_[folder];
        auto initValue = readSensorValue(str);
        auto tag = tagList()->createTag("temperature", name, Tag::eDouble, initValue);
        temperatureSensors_.emplace(str, tag);
        qDebug() << folder;
    }

    if(!temperatureSensors_.empty())
        readSensor_ = 0;

    return true;
}

void Ds18b20::createApi(QHttpServer &httpserver)
{

}

void Ds18b20::mainloop()
{
    if(readSensor_ < -1)
        return;

    auto iter = temperatureSensors_.begin();
    std::advance(iter, readSensor_);
    if(iter == temperatureSensors_.end())
    {
        iter = temperatureSensors_.begin();
        readSensor_ = 0;
    }
    readSensor_++;

    auto &[sensor, tag] = *iter;
    auto temperature = readSensorValue(sensor);
    tag->setValue(temperature);


}

double Ds18b20::readSensorValue(const QString &filePath)
{
    // read the file: sensor/w1_slave
    double sensorValue = 0.0;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        auto data = QString(file.readAll());
        auto tempeatureStr = data.split("t=").last();
        bool ok = false;
        int tempeature = tempeatureStr.toInt(&ok);
        if(ok)
        {
            sensorValue = tempeature / 1000.;
        }
        else
            qDebug() << data;
    }
    file.close();
    return sensorValue;
}
} // end namespace
