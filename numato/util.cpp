#include "util.h"


namespace util::numato {

std::bitset<16> hexToBitset(const QByteArray &data)
{
    QString binary = hexToBinary(Hex(data.at((0)))) + hexToBinary(Hex(data.at(1)))
                     + hexToBinary(Hex(data.at(2))) + hexToBinary(Hex(data.at(3)));
    return std::bitset<16>{binary.toLatin1().toStdString()};
}

QString hexToBinary(Hex hex)
{
    switch (hex) {
    case util::numato::Hex::Hex_0:
        return "0000";
    case util::numato::Hex::Hex_1:
        return "0001";
    case util::numato::Hex::Hex_2:
        return "0010";
    case util::numato::Hex::Hex_3:
        return "0011";
    case util::numato::Hex::Hex_4:
        return "0100";
    case util::numato::Hex::Hex_5:
        return "0101";
    case util::numato::Hex::Hex_6:
        return "0110";
    case util::numato::Hex::Hex_7:
        return "0111";
    case util::numato::Hex::Hex_8:
        return "1000";
    case util::numato::Hex::Hex_9:
        return "1001";
    case util::numato::Hex::Hex_A:
        return "1010";
    case util::numato::Hex::Hex_B:
        return "1011";
    case util::numato::Hex::Hex_C:
        return "1100";
    case util::numato::Hex::Hex_D:
        return "1101";
    case util::numato::Hex::Hex_E:
        return "1110";
    case util::numato::Hex::Hex_F:
        return "1111";
    }

    return QString("");
}

QString gpioToString(Gpio gpio)
{
    switch (gpio) {
    case  Gpio::Gpio_0:
        return QString("0");
    case  Gpio::Gpio_1:
        return QString("1");
    case  Gpio::Gpio_2:
        return QString("2");
    case  Gpio::Gpio_3:
        return QString("3");
    case  Gpio::Gpio_4:
        return QString("4");
    case  Gpio::Gpio_5:
        return QString("5");
    case  Gpio::Gpio_6:
        return QString("6");
    case  Gpio::Gpio_7:
        return QString("7");
    case  Gpio::Gpio_8:
        return QString("8");
    case  Gpio::Gpio_9:
        return QString("9");
    case  Gpio::Gpio_10:
        return QString("A");
    case  Gpio::Gpio_11:
        return QString("B");
    case  Gpio::Gpio_12:
        return QString("C");
    case  Gpio::Gpio_13:
        return QString("D");
    case  Gpio::Gpio_14:
        return QString("E");
    case  Gpio::Gpio_15:
        return QString("F");
    }
    return QString("");
}

Gpio intToGpio(int gpio)
{
    if(gpio == 0)
        return Gpio::Gpio_0;
    else if(gpio == 1)
        return Gpio::Gpio_1;
    else if(gpio == 2)
        return Gpio::Gpio_2;
    else if(gpio == 3)
        return Gpio::Gpio_3;
    else if(gpio == 4)
        return Gpio::Gpio_4;
    else if(gpio == 5)
        return Gpio::Gpio_5;
    else if(gpio == 6)
        return Gpio::Gpio_6;
    else if(gpio == 7)
        return Gpio::Gpio_7;
    else if(gpio == 8)
        return Gpio::Gpio_8;
    else if(gpio == 9)
        return Gpio::Gpio_9;
    else if(gpio == 10)
        return Gpio::Gpio_10;
    else if(gpio == 11)
        return Gpio::Gpio_11;
    else if(gpio == 12)
        return Gpio::Gpio_12;
    else if(gpio == 13)
        return Gpio::Gpio_13;
    else if(gpio == 14)
        return Gpio::Gpio_14;
    else if(gpio == 15)
        return Gpio::Gpio_15;

    Q_UNREACHABLE();
}

} // end namespace
