#ifndef PIGPIOAPI_H
#define PIGPIOAPI_H

class QHttpServer;
class QHttpServerResponse;
class QHttpServerRequest;
class QString;

namespace plugin{

class PiGpioApi
{
public:
    explicit PiGpioApi(const QString &configFileName);

    void setupApi(QHttpServer &httpserver);

private:
    QHttpServerResponse get(const QHttpServerRequest &request);

    const QString &configileName_;
};

} //end namespace
#endif // PIGPIOAPI_H
