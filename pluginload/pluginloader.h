#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "plugininterface.h"

#include <QString>

namespace pluginloader
{
    // Uses QPluginLoader to load as qt plugin
    PluginInterface* load(const QString &path, const QString &plugin);
}

#endif // PLUGINLOADER_H
