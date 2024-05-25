#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QString>
#include <tagsystem/taglist.h>

#include <plugins/pluginload/pluginloader.h>

class App : public QCoreApplication
{
public:
    App(int argc, char *argv[]);

private slots:
    void loadPlugins();

private:
    void loadPlugin(const QString &name);
    QString pluginName_;

    TagList &tagList_ = TagList::sGetInstance();

    PluginLoader pluginLoader_;
    PluginInterface *plugin_ = nullptr;
};

#endif // APP_H
