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

#include "util.h"

namespace plugin {

class Numato : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.numato")
public:
    Numato() = default;

    bool initialize() final;

    void read(util::numato::Gpio gpio);
    void set(util::numato::Gpio gpio);
    void clear(util::numato::Gpio gpio);
    void setIomask();
    void setIodir();
    void readAll();
    void notifyOn(bool enable);
    void adcRead(util::numato::Gpio gpio); ///< gpio 0-9 can be analog input

private slots:
    void mainloop() final;

    void onDataReady(const char*, int);

private:
    QString bitsetToHex(const std::bitset<16> &bitset);
    int gpioNumberToInt(util::numato::Gpio gpio);
    void loadSettings();
    void handleNotifyReply(const QByteArray &array);

    std::unique_ptr<Telnet> telnet_;
    std::bitset<16> iomask_ = {0000000000000000}; // default all off
    std::bitset<16> iodir_ = {0000000000000000}; // 0-Output, 1-Input
    std::bitset<16> adcEnabled_ = {0000000000000000}; // lookup if gpio is analog input
    std::optional<util::numato::Gpio> readRequest_;

    // init from config file
    void initGpioPin(util::numato::Gpio gpio, QString iodir, bool value);
    const QString configFile_ = "config_numato.json";
    QString deviceAdress_;
    QString deviceName_;
    QString loginUserName_;
    QString loginPassword_;

    std::map<util::numato::Gpio, std::unique_ptr<TagSocket>> gpioOutput_;
    std::map<util::numato::Gpio, Tag*> gpioInput_;
};

}// end namespace plugin

#endif // NUMATO_H
