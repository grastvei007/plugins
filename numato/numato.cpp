#include "numato.h"

namespace plugin{

QString toString(Gpio gpio)
{
    switch (gpio) {
    case plugin::Gpio::Gpio_0:
        return QString("0");
    case plugin::Gpio::Gpio_1:
        return QString("1");
    case plugin::Gpio::Gpio_2:
        return QString("2");
    case plugin::Gpio::Gpio_3:
        return QString("3");
    case plugin::Gpio::Gpio_4:
        return QString("4");
    case plugin::Gpio::Gpio_5:
        return QString("5");
    case plugin::Gpio::Gpio_6:
        return QString("6");
    case plugin::Gpio::Gpio_7:
        return QString("7");
    case plugin::Gpio::Gpio_8:
        return QString("8");
    case plugin::Gpio::Gpio_9:
        return QString("9");
    case plugin::Gpio::Gpio_10:
        return QString("A");
    case plugin::Gpio::Gpio_11:
        return QString("B");
    case plugin::Gpio::Gpio_12:
        return QString("C");
    case plugin::Gpio::Gpio_13:
        return QString("D");
    case plugin::Gpio::Gpio_14:
        return QString("E");
    case plugin::Gpio::Gpio_15:
        return QString("F");
    }
    return QString("");
}


bool Numato::initialize()
{
    telnet_ = std::make_unique<Telnet>(this);

    connect(telnet_.get(), &Telnet::newData, this, &Numato::onDataReady);

    telnet_->connectToHost("192.168.1.44", 23);

    return true;
}

void Numato::read(Gpio gpio)
{
    QString str("gpio read ");
    str += toString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::set(Gpio gpio)
{
    QString str("gpio set ");
    str += toString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::clear(Gpio gpio)
{
    QString str("gpio clear ");
    str += toString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::mainloop()
{

}

void Numato::onDataReady(const char* buffer, int size)
{
    QByteArray data(buffer, size);
    qDebug() << data;
    if(data.contains("User Name"))
    {
        telnet_->sendData("admin");
        telnet_->sendData("\n");
        return;
    }
    if(data.contains("Password"))
    {
        telnet_->sendData("admin");
        telnet_->sendData("\n");
        return;
    }
}

} // end namespace plugin
