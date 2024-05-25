#ifndef PIGPIO_H
#define PIGPIO_H

#include <plugins/plugincore/plugin.h>
#include <plugins/pluginload/plugininterface.h>

#include <tagsystem/tag.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>

#include <QObject>
#include <QTimer>
#include <QVector>

class Pin;

namespace plugin{

class PiGpio : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.pigpio")
public:
    PiGpio(QObject *parent = nullptr) : Plugin(){}

    bool initialize() final;

private slots:
    void mainloop() final;

private:
    QVector<Pin*> pins_;
};

} // end maespace plugin
#endif // PIGPIO_H
