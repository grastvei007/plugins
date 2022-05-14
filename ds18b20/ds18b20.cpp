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

}

void Ds18b20::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

bool Ds18b20::initialize()
{
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

}
