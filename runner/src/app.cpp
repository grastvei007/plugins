#include "app.h"

#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QProcessEnvironment>



App::App(int argc, char *argv[]) : QCoreApplication(argc, argv)
{
    QCommandLineParser parser;

    QCommandLineOption serverIp("ip", "server-ip", "Connect to server");
    serverIp.setDefaultValue("localhost");
    parser.addOption(serverIp);

    QCommandLineOption module("module", "module-name", "The module to run");
    parser.addOption(module);

    parser.process(*this);

    tagList_.setClientName("runner");
    tagList_.connectToServer(parser.value(serverIp), 5000);

    loadPlugin(parser.value(module));

}

void App::loadPlugin(const QString &name)
{
    QProcessEnvironment env;
    QString value = env.value("DEV_LIBS");

    plugin_ = pluginLoader_.load(QString("%1%2").arg(value, name));
    if(!plugin_)
    {
        qDebug() << "error loading plugine " << QString("%1%2").arg(value, name);
        return;
    }
    plugin_->setTagSystem(&tagList_);
    plugin_->initialize();
    plugin_->run(1000);
}
