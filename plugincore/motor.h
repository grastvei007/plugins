#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>


namespace core{

class Motor : public QObject
{
    Q_OBJECT
public:
    Motor(int wireingPiPin, QObject *parent = nullptr);

    void setSpeed(int speed);
    void setMaximumSpeed(int max);
    void stop();

    bool isRunning() const;

private slots:
    void adjustSpeed();

private:
    int currentSpeed_ = 0;
    int targetSpeed_ = 0;

    const int min_ = 0; // min percent
    const int max_ = 100; // max percent
    int maxSpeed_ = 100; // adjustable max speed percent

    int rampUpTime_ = 1; // ramp up ms per percent

    int wireingPiPin_ = 0;
};

}// end namespace

#endif // MOTOR_H
