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

    setupHttpServer(5005);

    pluginName_ = parser.value(module);

    connect(&tagList_, &TagList::connected, this, &App::loadPlugins);

    TagSocketList::sGetInstance().setApplicationName(parser.value(clientName));
    TagSocketList::sGetInstance().loadBindingList();
    TagSocketList::sGetInstance().setAutoSave();

    tagList_.setClientName(parser.value(clientName));
	tagList_.setReconnectIfServerCloseConnection();
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
    plugin_->createApi(httpServer_);
    plugin_->run(1000);
}

void App::setupHttpServer(quint16 port)
{
    // setup all routes on httpserver
    httpServer_.route("/", []() {
        return "June rest api up an running";
    });

    httpServer_.setMissingHandler(this, [](const QHttpServerRequest& request,
                                           QHttpServerResponder &responder) {
        qDebug() << request.url();
    });
    tcpServer_ = std::make_unique<QTcpServer>();
    if(!tcpServer_->listen(QHostAddress::Any, port))
    {
        qDebug() << "Http server not running";
        return;
    }
    httpServer_.bind(tcpServer_.get());
}
