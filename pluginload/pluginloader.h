#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "plugininterface.h"

#include <QString>

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    typedef PluginInterface* (*createPlugin)();

    PluginLoader(QObject *parent=nullptr);
    ~PluginLoader() = default;
    // old uses QLibrary to load
    PluginInterface* load(const QString &pluginName);
    // Uses QPluginLoader to load as qt plugin
    PluginInterface* load(const QString &path, const QString &plugin);
};

#endif // PLUGINLOADER_H
