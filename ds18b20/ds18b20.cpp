#include "ds18b20.h"

#include <QTimer>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QDebug>

extern "C" PluginInterface* createPlugin()
{
  return new Ds18b20();
}

Ds18b20::Ds18b20()
{
    folderToName_.emplace("28-01203c3bc31a", "indoor");
}

void Ds18b20::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

bool Ds18b20::initialize()
{
    indoor_ = tagList_->createTag("temperature", "inside", Tag::eDouble, 10.0);


    const QString path = "/sys/bus/w1/devices";
    QDir dir(path);
    QStringList dirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

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
        auto tag = tagList_->createTag("temperature", name, Tag::eDouble);
        temperatureSensors_.emplace(str, tag);
        qDebug() << folder;
    }


    return true;
}

void Ds18b20::run(int deltaMs)
{
    if(mainLoopTimer_)
        mainLoopTimer_->deleteLater();

    mainLoopTimer_ = new QTimer();
    mainLoopTimer_->setInterval(deltaMs);
    QObject::connect(mainLoopTimer_, &QTimer::timeout, this, &Ds18b20::mainloop);

    mainLoopTimer_->start();
}

void Ds18b20::stop()
{

}

void Ds18b20::mainloop()
{
    for(auto &[sensor, tag] : temperatureSensors_)
    {
        // read the file: sensor/w1_slave
        QFile file(sensor);
        if(file.open(QIODevice::ReadOnly))
        {
            auto data = QString(file.readAll());
            auto tempeatureStr = data.split("t=").last();
            bool ok = false;
            int tempeature = tempeatureStr.toInt(&ok);
            if(ok)
            {
                double value = tempeature / 1000.;
                tag->setValue(value);
            }
            else
                qDebug() << data;
        }
        file.close();
    }
}
