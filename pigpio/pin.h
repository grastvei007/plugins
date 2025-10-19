#ifndef PIN_H
#define PIN_H

#include <QObject>

#include <tagsystem/tagsocket.h>
#include <tagsystem/tag.h>

#include "wiringpiwrapper.h"

class QJsonObject;

namespace plugin{

/**
 *      raspberry pi 3b+ gpio pins, wiring pi pins, and physical pin
 *
 *      GPIO    | wiring pi | physical
 *    ---------------------
 *      GPIO 0  |   0       | 11
 *      GPIO 1  |   1       | 12
 *      GPIO 2  |   2       | 13
 *      GPIO 3  |   3       | 15
 *      GPIO 4  |   4       | 16
 *      GPIO 5  |   5       | 18
 *      GPIO 6  |   6       | 22
 *      GPIO 7  |   7       | 7
 *
 *      GPIO 21 |   21      | 29
 *      GPIO 22 |   22      | 31
 *      GPIO 23 |   23      | 33
 *      GPIO 24 |   24      | 35
 *      GPIO 25 |   25      | 37
 *      GPIO 26 |   26      | 32
 *      GPIO 27 |   27      | 36
 *      GPIO 28 |   28      | 38
 *      GPIO 29 |   29      | 40
 */

class Pin : public QObject
{
    Q_OBJECT
public:
    Pin(TagSocket *tagSocket, Tag *tag, int pinNumber, WiringPi::PinDir dir);

    void setEnable(bool enable);
    void setDirection(WiringPi::PinDir dir);
    void hookupTagSocket(const QString &tagFullName);

    QJsonObject toJson() const;

    int wiringPiPin() const;

private slots:
    void onTagValueChanged(Tag *tag);
    void onTagSocketValueChanged(int value);

private:
    void digitalRead(int value);
    void setupCallback();
    void setupPin();
    QString dirToJsonValue(WiringPi::PinDir dir) const;

    TagSocket *tagSocket_;
    Tag *tag_;
    int pinNumber_;
    int currentReadValue_;
    WiringPi::PinDir direction_ = WiringPi::PinDir::eOutput;
    int value_ = 0;
    bool enabled_ = false;

};

} //end namespace
#endif // PIN_H
