#ifndef PIGPIO_H
#define PIGPIO_H

#include <plugins/plugincore/plugin.h>


#include <QVector>

class Pin;

namespace plugin{

/* Developed for rasperry pi 3B */
class PiGpio : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.pigpio")
public:
    PiGpio() = default;

    bool initialize() final;

private slots:
    void mainloop() final;

private:
    QVector<Pin*> pins_;
};

} // end maespace plugin
#endif // PIGPIO_H
