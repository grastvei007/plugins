#include "pigpio.h"
#include "wiringpiwrapper.h"
#include "pin.h"

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>


extern "C" PluginInterface* createPlugin()
{
  WiringPi::setup();
  return new PiGpio();
}


PiGpio::PiGpio()
{

}

void PiGpio::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

bool PiGpio::initialize()
{
    QString path = QDir::homePath() + QDir::separator() + ".config" + QDir::separator()+ "june";
    QDir dir(path);
    if(!dir.exists())
        return false;

    path.append(QDir::separator());
    path.append("pigpio.xml");

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening file: " << path;
        return false;
    }

    QXmlStreamReader stream(&file);
    while(!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == QString("gpio"))
                continue;
            if(stream.name() == QString("pin"))
            {
                int pinNumber = stream.attributes().value("pinnumber").toInt();
                int pinMode = stream.attributes().value("mode").toInt();
                QString tagName = QString("pin_%1").arg(QString::number(pinNumber));
                auto tag = tagList_->createTag("pigpio", tagName, Tag::eInt);

                auto tagSocket = TagSocket::createTagSocket("pigpio", tagName, TagSocket::eInt);
                tagSocket->hookupTag(tag);
                Pin *pin = new Pin(tagSocket, pinNumber, pinMode);
                pins_.push_back(pin);
            }
        }
    }
    if(stream.hasError())
    {
        qDebug() << __FUNCTION__ << stream.errorString();
    }


    return true;
}

void PiGpio::run(int deltaMs)
{
    if(mainLoopTimer_)
        mainLoopTimer_->deleteLater();
    deltaMs_ = deltaMs;

    mainLoopTimer_ = new QTimer();
    mainLoopTimer_->setInterval(deltaMs);
    QObject::connect(mainLoopTimer_, &QTimer::timeout, this, &PiGpio::mainloop);

    mainLoopTimer_->start();
}

void PiGpio::stop()
{

}

void PiGpio::mainloop()
{

}
