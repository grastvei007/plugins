#include "pluginloader.h"

#include <QDir>
#include <QLibrary>
#include <QDebug>


PluginLoader::PluginLoader()
{

}

PluginInterface *PluginLoader::load(const QString &pluginName)
{
    QString plugin(pluginName);
#ifdef QT_DEBUG
    plugin + "d";
#endif
    plugin + ".so";

    QString path = QDir().absolutePath().append(QString("/%1").arg(plugin));
    QLibrary lib(path);

    if(lib.load())
    {
        createPlugin create_lib = (createPlugin)lib.resolve("createPlugin");
        if(create_lib)
        {
            PluginInterface *pluginInterface = create_lib();
            qDebug() << "Plugin loaded: " << plugin;
            return pluginInterface;
        }
        else
            qDebug() << "Failed to create plugin " << lib.errorString();
    }
    else
        qDebug() << "Error loading plugin " << lib.errorString();

    return nullptr;
}
