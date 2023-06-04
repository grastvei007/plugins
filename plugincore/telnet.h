#ifndef TELNET_H
#define TELNET_H

#include <QTcpSocket>
#include <QByteArray>
#include <QSize>

class Telnet : public QTcpSocket
{
    Q_OBJECT
public:
    Telnet(QObject *parent = nullptr);

    void connectToHost(const QString &host, quint16 port);

    bool isConnected() const;

    void write(const unsigned char byte);
    void writeCustomCRLF();
    void writeCustomCR();
    void sendData(const QByteArray &ba);
    void sendWindowSize();

    bool isBinaryMode() const;

signals:
    void newData(const char *, int); ///< buffer, length
    void endOfRecord();
    void echoLocal(bool);

private slots:
    void onSocketError(QAbstractSocket::SocketError err);
    void onReadyRead();

private:
    enum class TelnetStates : int
    {
        STATE_DATA          = 0,
        STATE_IAC           = 1,
        STATE_IACSB         = 2,
        STATE_IACWILL       = 3,
        STATE_IACDO         = 4,
        STATE_IACWONT       = 5,
        STATE_IACDONT       = 6,
        STATE_IACSBIAC      = 7,
        STATE_IACSBDATA     = 8,
        STATE_IACSBDATAIAC  = 9,
        STATE_DATAR         = 10,
        STATE_DATAN         = 11
    };

    enum TelnetCodes : int
    {
        // (client<->server)
        IAC = 255,
        EOR = 239,
        WILL = 251,
        WONT = 252,
        DO = 253,
        DONT = 254,
        SB = 250,
        SE = 240,

        // (client->server)
        TELOPT_BINARY = 0,
        TELOPT_ECHO = 1,
        TELOPT_SGA = 2,
        TELOPT_EOR = 25,
        TELOPT_NAWS = 31,
        TELOPT_TTYPE = 24
    };

    enum class TelnetQualifiers : int
    {
        TELQUAL_IS = 0,
        TELQUAL_SEND = 1
    };

    static constexpr unsigned char IACWILL[2] = { TelnetCodes::IAC, TelnetCodes::WILL };
    static constexpr unsigned char IACWONT[2] = { TelnetCodes::IAC, TelnetCodes::WONT };
    static constexpr unsigned char IACDO[2]   = { TelnetCodes::IAC, TelnetCodes::DO };
    static constexpr unsigned char IACDONT[2] = { TelnetCodes::IAC, TelnetCodes::DONT };
    static constexpr unsigned char IACSB[2]   = { TelnetCodes::IAC, TelnetCodes::SB };
    static constexpr unsigned char IACSE[2]   = { TelnetCodes::IAC, TelnetCodes::SE };

    static constexpr unsigned char _sendCodeArray[2]   = { TelnetCodes::IAC, 0 };
    static constexpr unsigned char _arrCRLF[2]         = { 13, 10 };
    static constexpr unsigned char _arrCR[2]           = { 13, 0 };

    QSize oldWindowSize_;
    QSize windowSize_;

    TelnetStates telnetProtocolState_ = TelnetStates::STATE_DATA;

    void reply(unsigned char action, unsigned char reply);
    void handleSB();

    void sendSB(TelnetCodes code, unsigned char *arg, int len);

    int processTelnetProtocol(int bytes);
    void resetProtocol();

    void transpose(const QByteArray &buffer);

    static const int incomingBufferSize_ = 1500;
    char incomingBuffer_[incomingBufferSize_];
    char bufferProcessed_[incomingBufferSize_];

    unsigned char receivedDX_[256];     // What IAC DO(NT) request do we have received already ?
    unsigned char receivedWX_[256];     // What IAC WILL/WONT request do we have received already ?
    char sentDX_[256];         // What IAC DO/DONT request do we have sent already ?
    char sentWX_[256];         // What IAC WILL/WONT request do we have sent already ?

    QByteArray bufferSB_;
    int actualSB_;

};

#endif // TELNET_H
