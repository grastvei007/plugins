#ifndef UTIL_H
#define UTIL_H

#include <QByteArray>
#include <QString>

#include <bitset>

namespace util::numato{

enum class Gpio
{
    Gpio_0 = '0',
    Gpio_1 = '1',
    Gpio_2 = '2',
    Gpio_3 = '3',
    Gpio_4 = '4',
    Gpio_5 = '5',
    Gpio_6 = '6',
    Gpio_7 = '7',
    Gpio_8 = '8',
    Gpio_9 = '9',
    Gpio_10 = 'A',
    Gpio_11 = 'B',
    Gpio_12 = 'C',
    Gpio_13 = 'D',
    Gpio_14 = 'E',
    Gpio_15 = 'F'
};

QString gpioToString(Gpio gpio);

enum class Hex
{
    Hex_0 = '0',
    Hex_1 = '1',
    Hex_2 = '2',
    Hex_3 = '3',
    Hex_4 = '4',
    Hex_5 = '5',
    Hex_6 = '6',
    Hex_7 = '7',
    Hex_8 = '8',
    Hex_9 = '9',
    Hex_A = 'A',
    Hex_B = 'B',
    Hex_C = 'C',
    Hex_D = 'D',
    Hex_E = 'E',
    Hex_F = 'F'
};

Gpio intToGpio(int gpio);
QString hexToBinary(Hex hex); ///< [0-9] [A-E]
std::bitset<16> hexToBitset(const QByteArray &data);
int gpioNumberToInt(util::numato::Gpio gpio);
QString bitsetToHex(const std::bitset<16> &bitset);

} // end namespace

#endif // UTIL_H
