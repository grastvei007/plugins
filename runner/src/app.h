#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QString>
#include <QHttpServer>
#include <QTcpServer>
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
    void setupHttpServer(quint16 port);
    QString pluginName_;

    TagList &tagList_ = TagList::sGetInstance();

    PluginInterface *plugin_ = nullptr;
    QHttpServer httpServer_;
    std::unique_ptr<QTcpServer> tcpServer_;
};

#endif // APP_H
