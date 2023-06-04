#include "telnet.h"

#include <QDebug>

// https://github.com/silderan/QTelnet/blob/master/QTelnet.h
// https://github.com/silderan/QTelnet/blob/master/QTelnet.cpp


Telnet::Telnet(QObject *parent) : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::errorOccurred, this, &Telnet::onSocketError);
    connect(this, &QTcpSocket::readyRead, this, &Telnet::onReadyRead);
}


void Telnet::connectToHost(const QString &host, quint16 port)
{
    if(isConnected())
        return;

    resetProtocol();
    abort();
    QTcpSocket::connectToHost(host, port);
}

bool Telnet::isConnected() const
{
    return state() == QAbstractSocket::ConnectedState;
}

void Telnet::sendData(const QByteArray &ba)
{
    if(isConnected())
        transpose(ba);
}

void Telnet::sendWindowSize()
{
    if( isConnected() && (receivedDX_[TELOPT_NAWS] == DO) && (oldWindowSize_ != windowSize_) )
    {
        unsigned char size[4];

        oldWindowSize_ = windowSize_;
        size[0] = (windowSize_.width()>>8) & 0xFF;
        size[1] = windowSize_.width() & 0xFF;
        size[2] = (windowSize_.height()>>8) & 0xFF;
        size[3] = windowSize_.height() & 0xFF;

        sendSB(TelnetCodes::TELOPT_NAWS, size, 4);
    }
}

bool Telnet::isBinaryMode() const
{
    return receivedDX_[static_cast<unsigned char>(TelnetCodes::TELOPT_BINARY)] == TelnetCodes::DO;
}


void Telnet::onSocketError(SocketError err)
{
    qDebug() << __FUNCTION__ << err;
    disconnectFromHost();
}

void Telnet::onReadyRead()
{
    while(int dataRead = read(incomingBuffer_, incomingBufferSize_ ) != 0)
    {
        switch (dataRead) {
        case -1:
            disconnectFromHost();
            break;
        default:
            if(int processed = processTelnetProtocol(dataRead); processed > 0)
                emit newData(incomingBuffer_, processed);

            break;
        }
    }
}

void Telnet::reply(unsigned char action, unsigned char reply)
{
    switch(reply)
    {
    case TelnetCodes::DO:
    {
        if( (reply != sentWX_[(unsigned char)action]) || (receivedDX_[(unsigned char)action != TelnetCodes::DO]) )
        {
            write(TelnetCodes::IAC);
            write(reply);
            write(action);

            sentWX_[(unsigned char)action] = reply;
            receivedDX_[(unsigned char)action] = static_cast<unsigned char>(TelnetCodes::DO);
        }
        break;
    }
    case TelnetCodes::DONT:
    {
        if( (reply != sentWX_[(unsigned char)action]) || (receivedDX_[(unsigned char)action] != TelnetCodes::DONT) )
        {
            write(IAC);
            write(reply);
            write(action);

            sentWX_[(unsigned char)action] = reply;
            receivedDX_[(unsigned char)action] = static_cast<unsigned char>(TelnetCodes::DONT);
        }
        break;
    }
    case TelnetCodes::WILL:
    {
        if( (reply != sentDX_[(unsigned char)action]) || (receivedWX_[(unsigned char)action] != static_cast<unsigned char>(TelnetCodes::WILL)) )
        {
            write(TelnetCodes::IAC);
            write(reply);
            write(action);

            sentDX_[(unsigned char)action] = reply;
            receivedWX_[(unsigned char)action] = static_cast<unsigned char>(TelnetCodes::WILL);
        }
        break;
    }
    case TelnetCodes::WONT:
    {
        if( (reply != sentDX_[(unsigned char)action]) || (receivedWX_[(unsigned char)action] != static_cast<unsigned char>(TelnetCodes::WONT)))
        {
            write(TelnetCodes::IAC);
            write(reply);
            write(action);

            sentDX_[(unsigned char)action] = reply;
            receivedWX_[(unsigned char)action] = static_cast<unsigned char>(TelnetCodes::WONT);
        }
        break;
    }
    default:
        break;
    }

}

void Telnet::write(const unsigned char byte)
{
    QTcpSocket::write( (char*)&byte, 1);
}

void Telnet::writeCustomCRLF()
{
    QTcpSocket::write((char*)_arrCRLF, 2);
}

void Telnet::writeCustomCR()
{
    QTcpSocket::write((char*)_arrCR, 2);
}

void Telnet::handleSB()
{
    if(actualSB_ == TelnetCodes::TELOPT_TTYPE)
    {
        if( (bufferSB_.size() > 0) && ((unsigned char)bufferSB_[0] == (unsigned char)TelnetQualifiers::TELQUAL_SEND) )
        {
            QTcpSocket::write((char*)&IACSB, 2);
            write(TELOPT_TTYPE);
            write((char)TelnetQualifiers::TELQUAL_IS);
            /* FIXME: need more logic here if we use
            * more than one terminal type
            */
            QTcpSocket::write("SiraggaTerminal", 15);
            QTcpSocket::write((char*)&IACSE, 2);
        }
    }
}

void Telnet::sendSB(TelnetCodes code, unsigned char *arg, int len)
{
    write(TelnetCodes::IAC);
    write(TelnetCodes::SB);
    write(code);

    QTcpSocket::write((char*)&arg, len);

    write(TelnetCodes::IAC);
    write(TelnetCodes::SE);
}

int Telnet::processTelnetProtocol(int bufferSize)
{
    int bytesOut = 0;

    for(int bytesIn = 0; bytesIn < bufferSize; ++bytesIn)
    {
        unsigned char byte = incomingBuffer_[bytesIn];

        switch (telnetProtocolState_)
        {
        case TelnetStates::STATE_DATA:
        {
            switch (byte)
            {
            case TelnetCodes::IAC:
                telnetProtocolState_ = TelnetStates::STATE_IAC;
                break;

            case '\r':
                telnetProtocolState_ = TelnetStates::STATE_DATAR;
                break;
            case '\n':
                telnetProtocolState_ = TelnetStates::STATE_DATAN;
                break;
            default:
                bufferProcessed_[bytesOut++] = byte;
                break;
            }
            break;
        }
        case TelnetStates::STATE_DATAR:
        case TelnetStates::STATE_DATAN:
        {
            switch (byte)
            {
            case TelnetCodes::IAC:
                telnetProtocolState_ = TelnetStates::STATE_IAC;
                break;
            case '\r':
            case '\n':
                bufferProcessed_[bytesOut++] = '\n';
                telnetProtocolState_ = TelnetStates::STATE_DATA;
                break;
            default:
                bufferProcessed_[bytesOut++] = byte;
                telnetProtocolState_ = TelnetStates::STATE_DATA;
                break;
            }
            break;
        }
        case TelnetStates::STATE_IAC:
        {
            switch(byte)
            {
                case TelnetCodes::IAC:
                    telnetProtocolState_ = TelnetStates::STATE_DATA;
                    bufferProcessed_[bytesOut++] = static_cast<unsigned char>(TelnetCodes::IAC);
                    break;
                case TelnetCodes::WILL:
                    telnetProtocolState_ = TelnetStates::STATE_IACWILL;
                    break;
                case TelnetCodes::WONT:
                    telnetProtocolState_ = TelnetStates::STATE_IACWONT;
                    break;
                case TelnetCodes::DONT:
                    telnetProtocolState_ = TelnetStates::STATE_IACDONT;
                    break;
                case TelnetCodes::DO:
                    telnetProtocolState_ = TelnetStates::STATE_IACDO;
                    break;
                case TelnetCodes::EOR:
                    emit endOfRecord();
                    telnetProtocolState_ = TelnetStates::STATE_DATA;
                    break;
                case TelnetCodes::SB:
                {
                    telnetProtocolState_ = TelnetStates::STATE_IACSB;
                    bufferSB_.clear();
                    break;
                }
                default:
                    telnetProtocolState_ = TelnetStates::STATE_DATA;
                    break;
            }
            break;
        }
        case TelnetStates::STATE_IACWILL:
        {
            switch(byte)
            {
            case TelnetCodes::TELOPT_BINARY:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DO));
                    break;
            case TelnetCodes::TELOPT_ECHO:
                    emit echoLocal(false);
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DO));
                    break;
            case TelnetCodes::TELOPT_SGA:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DO));
                    break;
            case TelnetCodes::TELOPT_EOR:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DO));
                    break;
            default:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DONT));
                    break;
            }
            telnetProtocolState_ = TelnetStates::STATE_DATA;
            break;
        }
        case TelnetStates::STATE_IACWONT:
        {
            switch(byte)
            {
            case TelnetCodes::TELOPT_BINARY:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DONT));
                    break;
            case TelnetCodes::TELOPT_ECHO:
                    emit echoLocal(true);
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DONT));
                    break;
            case TelnetCodes::TELOPT_SGA:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DONT));
                    break;
            case TelnetCodes::TELOPT_EOR:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DONT));
                    break;
            default:
                    reply(byte, static_cast<unsigned char>(TelnetCodes::DONT));
                    break;
            }
            telnetProtocolState_ = TelnetStates::STATE_DATA;
            break;
        }
        case TelnetStates::STATE_IACDO:
        {
            switch(byte)
            {
            case TelnetCodes::TELOPT_ECHO:
                    reply(byte, TelnetCodes::WILL);
                    emit echoLocal(true);
                    break;
            case TelnetCodes::TELOPT_SGA:
                    reply(byte, TelnetCodes::WILL);
                    break;
            case TelnetCodes::TELOPT_TTYPE:
                    reply(byte, TelnetCodes::WILL);
                    break;
            case TelnetCodes::TELOPT_BINARY:
                    reply(byte, TelnetCodes::WILL);
                    break;
            case TelnetCodes::TELOPT_NAWS:
                    receivedDX_[(unsigned char)byte] = (unsigned char)TelnetCodes::DO;
                    sentWX_[(unsigned char)byte] = (unsigned char)TelnetCodes::WILL;
                    write(static_cast<unsigned char>(TelnetCodes::IAC));
                    write(TelnetCodes::WILL);
                    write(byte);

                  //  sendWindowSize();
                    break;
            default:
                    reply(byte, TelnetCodes::WONT);
                    break;
            }
            telnetProtocolState_ = TelnetStates::STATE_DATA;
            break;
        }
        case TelnetStates::STATE_IACDONT:
        {
            switch (byte)
            {
            case TelnetCodes::TELOPT_ECHO:
                    reply(byte, TelnetCodes::WONT);
                    emit echoLocal(false);
                    break;
            case TelnetCodes::TELOPT_SGA:
                    reply(byte, TelnetCodes::WONT);
                    break;
            case TelnetCodes::TELOPT_NAWS:
                    reply(byte, TelnetCodes::WONT);
                    break;
            case TelnetCodes::TELOPT_BINARY:
                    reply(byte, TelnetCodes::WONT);
                    break;
            default:
                    reply(byte, TelnetCodes::WONT);
                    break;
            }
            telnetProtocolState_ = TelnetStates::STATE_DATA;
            break;
        }
        case TelnetStates::STATE_IACSB:
        {
            switch(byte)
            {
            case TelnetCodes::IAC:
                    telnetProtocolState_ = TelnetStates::STATE_IACSBIAC;
                    break;
            default:
                    bufferSB_.clear();
                    actualSB_ = byte;
                    telnetProtocolState_ = TelnetStates::STATE_IACSBDATA;
                    break;
            }
            break;
        }
        case TelnetStates::STATE_IACSBDATA:
        {
            switch(byte)
            {
            case TelnetCodes::IAC:
                    telnetProtocolState_ = TelnetStates::STATE_IACSBDATAIAC;
                    break;
            default:
                    bufferSB_.append(byte);
                    break;
            }
            break;
        }
        case TelnetStates::STATE_IACSBIAC:
        {
            switch(byte)
            {
            case TelnetCodes::IAC:
                    bufferSB_.clear();
                    actualSB_ = byte;
                    telnetProtocolState_ = TelnetStates::STATE_IACSBDATA;
            default:
                    telnetProtocolState_ = TelnetStates::STATE_DATA;
            }
            break;
        }
        case TelnetStates::STATE_IACSBDATAIAC:
        {
            switch(byte)
            {
            case TelnetCodes::IAC:
                    telnetProtocolState_ = TelnetStates::STATE_IACSBDATA;
                    bufferSB_.append(static_cast<unsigned char>(TelnetCodes::IAC));
                    break;
            case TelnetCodes::SE:
                    handleSB();
                    actualSB_ = 0;
                    bufferSB_.clear();
                    telnetProtocolState_ = TelnetStates::STATE_DATA;
                    break;
            case TelnetCodes::SB:
                    handleSB();
                    bufferSB_.clear();
                    telnetProtocolState_ = TelnetStates::STATE_IACSB;
                    break;
            default:
                    bufferSB_.clear();
                    actualSB_ = 0;
                    telnetProtocolState_ = TelnetStates::STATE_DATA;
                    break;
            }
            break;
        }

        default:
            telnetProtocolState_ = TelnetStates::STATE_DATA;
            break;
        }
    }

    return bytesOut;
}

void Telnet::resetProtocol()
{
    telnetProtocolState_ = TelnetStates::STATE_DATA;
}

void Telnet::transpose(const QByteArray &buffer)
{
    for(unsigned char byte : buffer)
    {
        switch(byte)
        {
        case TelnetCodes::IAC:
            write(TelnetCodes::IAC);
            write(TelnetCodes::IAC);
            break;
        case 10: // \n
        {
            if(isBinaryMode())
                write(byte);
            else
                writeCustomCRLF();

            break;
        }
        case 13: // \r
        {
            if(isBinaryMode())
                write(byte);
            else
                writeCustomCR();

            break;
        }
        default:
            write(byte);
            break;
        }
    }
}
