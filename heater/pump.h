#ifndef PUMP_H
#define PUMP_H

#include <QTimer>
#include <QObject>

class Pump : public QObject
{
    Q_OBJECT
public:
    Pump(){};
    Pump(int wireingPiPin);

    void setInterval(double intervalInSeconds);
    void setSpeed(int speed);
    void start();
    void stop();

    bool isRunning() const;

private slots:
    void onPumpTimerTimeout();

private:
    int currentSpeed_ = 0;
    double pumpIntervalSeconds_ = 5;
    double pumpMinInterValSeconds_ = 1;
    double pumpMaxIntervalSeconds_ = 5;
    bool running_ = false;

    int wireingpiPin_ = 0;

    QTimer *pumpTimer_ = nullptr;
};

#endif // PUMP_H
