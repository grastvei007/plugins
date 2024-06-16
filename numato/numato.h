#ifndef NUMATO_H
#define NUMATO_H

#include <plugins/plugincore/plugin.h>
#include <plugins/plugincore/telnet.h>

#include <QObject>

#include <memory>

namespace plugin {

class Numato : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.numato")
public:
    Numato() = default;

    bool initialize() final;

private slots:
    void mainloop() final;

    void onDataReady(const char*, int);

private:
    std::unique_ptr<Telnet> telnet_;
};

}// end namespace plugin

#endif // NUMATO_H
