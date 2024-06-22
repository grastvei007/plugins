#ifndef NUMATO_H
#define NUMATO_H

#include <plugins/plugincore/plugin.h>
#include <plugins/plugincore/telnet.h>

#include <tagsystem/tag.h>
#include <tagsystem/tagsocket.h>

#include <QObject>
#include <QString>

#include <memory>
#include <bitset>
#include <optional>
#include <map>

namespace plugin {

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

inline QString gpioToString(Gpio gpio)
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


class Numato : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.numato")
public:
    Numato() = default;

    bool initialize() final;

    void read(Gpio gpio);
    void set(Gpio gpio);
    void clear(Gpio gpio);
    void setIomask();
    void setIodir();
    void readAll();
    void notifyOn(bool enable);
    void adcRead(Gpio gpio); ///< gpio 0-9 can be analog input

private slots:
    void mainloop() final;

    void onDataReady(const char*, int);

private:
    QString bitsetToHex(const std::bitset<16> &bitset);
    int gpioNumberToInt(Gpio gpio);
    void loadSettings();

    std::unique_ptr<Telnet> telnet_;
    std::bitset<16> iomask_ = {0000000000000000}; // default all off
    std::bitset<16> iodir_ = {0000000000000000}; // 0-Output, 1-Input
    std::bitset<16> adcEnabled_ = {0000000000000000}; // lookup if gpio is analog input
    std::optional<Gpio> readRequest_;

    // init from config file
    void initGpioPin(Gpio gpio, QString iodir, bool value);
    const QString configFile_ = "config_numato.json";
    QString deviceAdress_;
    QString deviceName_;
    QString loginUserName_;
    QString loginPassword_;

    std::map<Gpio, std::unique_ptr<TagSocket>> gpioOutput_;
    std::map<Gpio, Tag*> gpioInput_;
};

}// end namespace plugin

#endif // NUMATO_H
