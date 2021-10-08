#include "pluginloader.h"

#include <QDir>
#include <QLibrary>
#include <QDebug>


PluginLoader::PluginLoader()
{

}

PluginInterface *PluginLoader::load(const QString &pluginName)
{
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
