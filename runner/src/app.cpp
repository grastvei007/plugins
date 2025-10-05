#include "app.h"
#include "tagsystem/tagsocketlist.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QSettings>
#include <QString>

#include <plugins/pluginload/pluginloader.h>

App::App(int argc, char *argv[]) : QCoreApplication(argc, argv)
{
    QCommandLineParser parser;

    QCommandLineOption serverIp("ip", "server-ip", "Connect to server");
    serverIp.setDefaultValue("localhost");
    parser.addOption(serverIp);

    QCommandLineOption module("module", "module-name", "The module to run");
    parser.addOption(module);

    QCommandLineOption clientName("client", "client-name", "The name of this instance when connected to server.");
    clientName.setDefaultValue("runner");
    parser.addOption(clientName);

    parser.process(*this);

    pluginName_ = parser.value(module);

    connect(&tagList_, &TagList::connected, this, &App::loadPlugins);

    TagSocketList::sGetInstance().setApplicationName("runner");
    TagSocketList::sGetInstance().loadBindingList();

    tagList_.setClientName(parser.value(clientName));
    if(!TagList::sGetInstance().tryToAutoConnect())
        TagList::sGetInstance().connectToServer(parser.value(serverIp), 5000);
}

void App::loadPlugins()
{
    if(!pluginName_.isEmpty())
        loadPlugin(pluginName_);
}

void App::loadPlugin(const QString &name)
{
    QSettings settings("june", "june");
    QString pluginPath = settings.value("global/plugindir").toString() + "/";

    if(pluginPath.isEmpty())
    {
        qFatal() << "Error, add plugindir in section global in config file june.conf";
        return;
    }

    plugin_ = pluginloader::load(pluginPath, name);
    if(!plugin_)
    {
        qDebug() << "error loading plugine " << QString("%1%2").arg(pluginPath, name);
        return;
    }
    plugin_->setTagSystem(&tagList_);
    plugin_->initialize();
    plugin_->run(1000);
}
