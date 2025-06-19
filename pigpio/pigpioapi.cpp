#include "pigpioapi.h"
#include "pigpio.h"
#include <QHttpServer>
#include <QHttpServerResponse>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <plugins/plugincore/path.h>

namespace plugin {

PiGpioApi::PiGpioApi(PiGpio &piGpio)
    : piGpio_(piGpio)
{}

void PiGpioApi::setupApi(QHttpServer &httpserver)
{
    httpserver.route("/api/gpio/get",
                     QHttpServerRequest::Method::Get,
                     [this](const QHttpServerRequest &request) { return get(request); });
}

QHttpServerResponse PiGpioApi::get(const QHttpServerRequest &request)
{
    return QHttpServerResponse(piGpio_.toJson());
}

} // end namespave
