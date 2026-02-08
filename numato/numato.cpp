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

void Numato::read(util::numato::Gpio gpio)
{
    QString str("gpio read ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::set(util::numato::Gpio gpio)
{
    QString str("gpio set ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::clear(util::numato::Gpio gpio)
{
    QString str("gpio clear ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::setIomask()
{
    QString str("gpio iomask ");
    str += util::numato::bitsetToHex(iomask_);
    qDebug() << __FUNCTION__ << str;
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::setIodir()
{
    QString str("gpio iodir ");
    str += util::numato::bitsetToHex(iodir_);
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

void Numato::adcRead(util::numato::Gpio gpio)
{
    // Filter out invalid gpio
    if(gpio == util::numato::Gpio::Gpio_10 || gpio == util::numato::Gpio::Gpio_11 || gpio == util::numato::Gpio::Gpio_12
        || gpio == util::numato::Gpio::Gpio_13 || gpio == util::numato::Gpio::Gpio_14 || gpio == util::numato::Gpio::Gpio_15)
        return;

    QString str("adc read ");
    str += gpioToString(gpio);
    telnet_->sendData(str.toLatin1());
    telnet_->sendData("\n");
}

void Numato::loop() {}

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
        // if there are set some input turn on notification on changes.
        if(iodir_.any())
            notifyOn(true);
        return;
    }

    if(readRequest_.has_value()) // response to read gpio input or analog
    {
        auto gpio = readRequest_.value();
        // is analog read
        if(adcEnabled_.test(util::numato::gpioNumberToInt(gpio)))
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
        handleNotifyReply(data);
    }
}

void Numato::handleNotifyReply(const QByteArray &array)
{
    auto current = util::numato::hexToBitset(array.sliced(0, 4));
    auto previous = util::numato::hexToBitset(array.sliced(4, 4));
    auto iodir = util::numato::hexToBitset(array.sliced(8, 4));

    for(int i = 0; i<iodir.size(); ++i)
    {
        if(iodir.test(i) == 0) // output
            continue;

        // gpio input changed
        if(current.test(i) != previous.test(i))
        {
            bool value = current.test(1) == 1 ? true : false;
            auto gpio = util::numato::intToGpio(i);
            if(gpioInput_.contains(gpio))
                gpioInput_[gpio]->setValue(value);
        }
    }

}

} // end namespace plugin
