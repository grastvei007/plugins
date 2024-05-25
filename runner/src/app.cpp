#include "app.h"
#include "tagsystem/tagsocketlist.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QProcessEnvironment>
#include <QString>


App::App(int argc, char *argv[]) : QCoreApplication(argc, argv)
{
    QCommandLineParser parser;

    QCommandLineOption serverIp("ip", "server-ip", "Connect to server");
    serverIp.setDefaultValue("localhost");
    parser.addOption(serverIp);

    QCommandLineOption module("module", "module-name", "The module to run");
    parser.addOption(module);

    parser.process(*this);

    pluginName_ = parser.value(module);

    connect(&tagList_, &TagList::connected, this, &App::loadPlugins);

    TagSocketList::sGetInstance().setApplicationName("runner");
    TagSocketList::sGetInstance().loadBindingList();

    tagList_.setClientName("runner");
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
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString value = env.value("DEV_LIBS") + "/";
    qDebug() << "DEV_LIBS " << value;
    plugin_ = pluginLoader_.load(value, name);
    if(!plugin_)
    {
        qDebug() << "error loading plugine " << QString("%1%2").arg(value, name);
        return;
    }
    plugin_->setTagSystem(&tagList_);
    plugin_->initialize();
    plugin_->run(1000);
}
