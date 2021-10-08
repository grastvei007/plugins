#ifndef MOTOR_H
#define MOTOR_H


class Motor
{
public:
    Motor(int wireingPiPin);

    void turnOn();
    void turnOff();

    void setSpeed(int speed);

    bool isRunning() const;
    bool isOn() const;
private:
    int currentSpeed_ = 0;
    int minSpeed_ = 0;
    int maxSpeed_ = 100;

    bool isOn_ = false;

    int wireingPiPin_ = 0;
};

#endif // MOTOR_H
