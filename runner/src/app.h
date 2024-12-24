#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QString>
#include <tagsystem/taglist.h>

class PluginInterface;

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

    PluginInterface *plugin_ = nullptr;
};

#endif // APP_H
