#ifndef NUMATO_H
#define NUMATO_H

#include <plugins/plugincore/plugin.h>

#include <QObject>

class Numato : public Plugin
{
    Q_OBJECT
public:
    Numato() = default;

    bool initialize() final;

private slots:
    void mainloop() final;
};

#endif // NUMATO_H
