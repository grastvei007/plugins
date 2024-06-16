#include "numato.h"
#include <QString>

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

    qDebug() << bitsetToHex(iomask_);

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

void Numato::setIomask()
{
    QString str("gpio iomask ");
    str += bitsetToHex(iomask_);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::setIodir()
{
    QString str("gpio iodir ");
    str += bitsetToHex(iodir_);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::readAll()
{
    QString str("gpio readall");
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::notifyOn(bool enable)
{
    QString str("gpio notify ");
    str += enable ? "on" : "off";
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::adcRead(Gpio gpio)
{
    // Filter out invalid gpio
    if(gpio == Gpio::Gpio_10 || gpio == Gpio::Gpio_11 || gpio == Gpio::Gpio_12
        || gpio == Gpio::Gpio_13 || gpio == Gpio::Gpio_14 || gpio == Gpio::Gpio_15)
        return;

    QString str("adc read ");
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

QString Numato::bitsetToHex(const std::bitset<16> &bitset)
{
    auto toHex = [](int i)
    {
        if(i == 0)
            return "0";
        else if(i == 1)
            return "1";
        else if(i == 2)
            return "2";
        else if(i == 3)
            return "3";
        else if(i == 4)
            return "4";
        else if(i == 5)
            return "5";
        else if(i == 6)
            return "6";
        else if(i == 7)
            return "7";
        else if(i == 8)
            return "8";
        else if(i == 9)
            return "9";
        else if(i == 10)
            return "A";
        else if(i == 11)
            return "B";
        else if(i == 12)
            return "C";
        else if(i == 13)
            return "D";
        else if(i == 14)
            return "E";
        else if(i == 15)
            return "F";

        return "";
    };

    QString hexString;

    auto string = QString::fromStdString(bitset.to_string());
    for(int i = 0; i < 16; i+=4)
    {
        if((i + 4) > 16)
            break;
        QString str = string.sliced(i, 4);
        std::bitset<4> set(str.toLatin1().toStdString());
        hexString.append(toHex(set.to_ulong()));
    }

    return hexString;
}

} // end namespace plugin
