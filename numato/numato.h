#ifndef NUMATO_H
#define NUMATO_H

#include <plugins/plugincore/plugin.h>
#include <plugins/plugincore/telnet.h>

#include <QObject>

#include <memory>

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

private slots:
    void mainloop() final;

    void onDataReady(const char*, int);

private:
    std::unique_ptr<Telnet> telnet_;
};

}// end namespace plugin

#endif // NUMATO_H
