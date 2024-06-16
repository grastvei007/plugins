#include "numato.h"

namespace plugin{

bool Numato::initialize()
{
    telnet_ = std::make_unique<Telnet>(this);

    connect(telnet_.get(), &Telnet::newData, this, &Numato::onDataReady);

    telnet_->connectToHost("192.168.1.44", 23);

    return true;
}

void Numato::mainloop()
{

}

void Numato::onDataReady(const char* buffer, int size)
{
    QByteArray data(buffer, size);
    qDebug() << data;
    if(data.contains("User Name"))
    {
        telnet_->sendData("admin");
        telnet_->sendData("\n");
    }
    if(data.contains("Password"))
    {
        telnet_->sendData("admin");
        telnet_->sendData("\n");
    }
}

} // end namespace plugin
