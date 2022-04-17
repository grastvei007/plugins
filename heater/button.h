#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include "wiringpiwrapper.h"

class Button : public QObject
{
    Q_OBJECT
public:
    explicit Button(int wiringPiPin, WiringPi::TriggerEdge trigger, QObject *parent = nullptr);

    static Button* instance();
signals:
    void buttonToggled(int level);
private:
    void onButtonPushed(int level);
    void setupCallback();
    int wiringPiPin_ = 0;
    WiringPi::TriggerEdge trigger_;
    int state_ = 0;
};


#endif // BUTTON_H
