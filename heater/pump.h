#ifndef PUMP_H
#define PUMP_H


class Pump
{
public:
    Pump();

    void setInterval(double intervalInSeconds);
    void setSpeed(int speed);
    void start();
    void stops();

    bool isRunning() const;
private:
    double pumpIntervalSeconds_ = 5;
    double pumpMinInterValSeconds_ = 1;
    double pumpMaxIntervalSeconds_ = 5;
    bool running_ = false;
};

#endif // PUMP_H
