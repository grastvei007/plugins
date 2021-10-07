#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "plugininterface.h"

#include <QString>

class PluginLoader
{
public:
    typedef PluginInterface* (*createPlugin)();
    PluginLoader();

    PluginInterface* load(const QString &pluginName);
};

#endif // PLUGINLOADER_H
