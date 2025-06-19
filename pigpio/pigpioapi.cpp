#include "pigpioapi.h"

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QString>
#include <QFile>
#include <QDebug>

#include <plugins/plugincore/path.h>

namespace plugin {

PiGpioApi::PiGpioApi(const QString &configFileName)
    : configileName_(configFileName)
{}

void PiGpioApi::setupApi(QHttpServer &httpserver)
{
    httpserver.route("/api/gpio/get",
                     QHttpServerRequest::Method::Get,
                     [this](const QHttpServerRequest &request) { return get(request); });
}

QHttpServerResponse PiGpioApi::get(const QHttpServerRequest &request)
{
    auto filepath = path::fromConfigDir(configileName_);
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        file.close();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
    }

    auto data = file.readAll();
    return QHttpServerResponse(data);
}

} // end namespave
