#include "pluginloader.h"

#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QDebug>
#include <QDir>

namespace pluginloader
{

PluginInterface* load(const QString &path, const QString &pluginName)
{
    QDir pluginPath(path);
    QPluginLoader pluginLoader(pluginPath.absoluteFilePath(pluginName));
    QObject *plugin = pluginLoader.instance();
    if(plugin)
    {
        PluginInterface *interface = qobject_cast<PluginInterface *>(plugin);
        if(interface)
            return interface;
        pluginLoader.unload();
    }
    else
        qDebug() << pluginLoader.errorString();
    return nullptr;
}


} // end namespace
