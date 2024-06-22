#include "numato.h"
#include <QString>

namespace plugin{

bool Numato::initialize()
{
    loadSettings();

    telnet_ = std::make_unique<Telnet>(this);

    connect(telnet_.get(), &Telnet::newData, this, &Numato::onDataReady);

    telnet_->connectToHost(deviceAdress_, 23);

    return true;
}

void Numato::read(Gpio gpio)
{
    QString str("gpio read ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::set(Gpio gpio)
{
    QString str("gpio set ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::clear(Gpio gpio)
{
    QString str("gpio clear ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::setIomask()
{
    QString str("gpio iomask ");
    str += bitsetToHex(iomask_);
    qDebug() << __FUNCTION__ << str;
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::setIodir()
{
    QString str("gpio iodir ");
    str += bitsetToHex(iodir_);
    qDebug() << __FUNCTION__ << str;
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
    str += gpioToString(gpio);
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
        telnet_->sendData(loginUserName_.toLatin1());
        telnet_->sendData("\n");
        return;
    }
    if(data.contains("Password"))
    {
        telnet_->sendData(loginPassword_.toLatin1());
        telnet_->sendData("\n");
        return;
    }
    // logged in.
    if(data.contains("successfully"))
    {
        // set pin config.
        setIomask();
        setIodir();
        return;
    }

    if(readRequest_.has_value()) // response to read gpio input or analog
    {
        auto gpio = readRequest_.value();
        // is analog read
        if(adcEnabled_.test(gpioNumberToInt(gpio)))
        {

        }
        else
        {

        }
        readRequest_.reset();
        return;
    }


    if(data.size() == 16) // response readall
    {

    }
    else if(data.size() == 48) // auto response for notify
    {

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

int Numato::gpioNumberToInt(Gpio gpio)
{
    switch (gpio) {
    case plugin::Gpio::Gpio_0:
        return 0;
    case plugin::Gpio::Gpio_1:
        return 1;
    case plugin::Gpio::Gpio_2:
        return 2;
    case plugin::Gpio::Gpio_3:
        return 3;
    case plugin::Gpio::Gpio_4:
        return 4;
    case plugin::Gpio::Gpio_5:
        return 5;
    case plugin::Gpio::Gpio_6:
        return 6;
    case plugin::Gpio::Gpio_7:
        return 7;
    case plugin::Gpio::Gpio_8:
        return 8;
    case plugin::Gpio::Gpio_9:
        return 9;
    case plugin::Gpio::Gpio_10:
        return 10;
    case plugin::Gpio::Gpio_11:
        return 11;
    case plugin::Gpio::Gpio_12:
        return 12;
    case plugin::Gpio::Gpio_13:
        return 13;
    case plugin::Gpio::Gpio_14:
        return 14;
    case plugin::Gpio::Gpio_15:
        return 15;
    }
    return 0;
}


} // end namespace plugin
