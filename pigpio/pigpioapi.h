#ifndef PIGPIOAPI_H
#define PIGPIOAPI_H

class QHttpServer;
class QHttpServerResponse;
class QHttpServerRequest;
class QString;

namespace plugin{

class PiGpio;

class PiGpioApi
{
public:
    explicit PiGpioApi(PiGpio &piGpio);

    void setupApi(QHttpServer &httpserver);

private:
    QHttpServerResponse get(const QHttpServerRequest &request);
    QHttpServerResponse update(const QHttpServerRequest &request);

    PiGpio &piGpio_;
};

} //end namespace
#endif // PIGPIOAPI_H
