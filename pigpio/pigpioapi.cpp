#include "pigpioapi.h"
#include "pigpio.h"
#include <QHttpServer>
#include <QHttpServerResponse>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <plugins/plugincore/path.h>

#include <tagsystem/util/json.h>

namespace plugin {

PiGpioApi::PiGpioApi(PiGpio &piGpio)
    : piGpio_(piGpio)
{}

void PiGpioApi::setupApi(QHttpServer &httpserver)
{
    httpserver.route("/api/gpio/get",
                     QHttpServerRequest::Method::Get,
                     [this](const QHttpServerRequest &request) { return get(request); });

    httpserver.route("/api/gpio/update",
                     QHttpServerRequest::Method::Post,
                     [this](const QHttpServerRequest &request) { return update(request); });
}

QHttpServerResponse PiGpioApi::get(const QHttpServerRequest &request)
{
    return QHttpServerResponse(piGpio_.toJson());
}

// requried:
// wiringpi: <pin>
// values that can be updated:
// enabled: <bool>
// dir: <in|out|pwm>
// hookup_tag: <string> full tag name
QHttpServerResponse PiGpioApi::update(const QHttpServerRequest &request)
{
    auto json = util::json::byteArrayToJsonObject(request.body());
    if (!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject object = json.value();
    if (!object.contains("wiringpi"))
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    if (!object.contains("dir") && !object.contains("enabled"))
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    int pinId = object.value("wiringpi").toInt();

    if (object.contains("dir"))
    {
        auto dir = object.value("dir").toString();
        auto dirEnum = piGpio_.dirToEnum(dir);
        if (dirEnum.has_value())
            piGpio_.updateDirection(pinId, dirEnum.value());
    }
    if (object.contains("enabled"))
    {
        auto enabled = object.value("enabled").toBool();
        piGpio_.updateEnable(pinId, enabled);
    }
    if(object.contains("hookup_tag"))
    {
        const auto tagFullName = object.value("hookup_tag").toString();
        piGpio_.updateTagSocketHookUpTag(pinId, tagFullName);
    }

    return QHttpServerResponse(piGpio_.pinToJson(pinId));
}

} // end namespave
