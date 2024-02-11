#include "pluginloader.h"

#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QDebug>
#include <QDir>

PluginLoader::PluginLoader(QObject *parent) : QObject(parent)
{

}

PluginInterface *PluginLoader::load(const QString &pluginName)
{
    qDebug() << pluginName;
    QLibrary lib(pluginName);

    if(lib.load())
    {
        createPlugin create_lib = (createPlugin)lib.resolve("createPlugin");
        if(create_lib)
        {
            PluginInterface *pluginInterface = create_lib();
            qDebug() << "Plugin loaded: " << pluginName;
            return pluginInterface;
        }
        else
            qDebug() << "Failed to create plugin " << lib.errorString();
    }
    else
        qDebug() << "Error loading plugin " << lib.errorString();

    return nullptr;
}

PluginInterface *PluginLoader::load(const QString &path, const QString &pluginName)
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
