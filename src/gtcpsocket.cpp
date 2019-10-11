#include "gtcpsocket.h"

#include <QDebug>

GTcpSocket::GTcpSocket(QObject *parent) :
  QTcpSocket(parent),
  host("127.0.0.1"),
  port(1234),
  connectionTimeout(5000)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    connect(this,SIGNAL(connected()),this,SLOT(connectedSlot()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));

    connect(this,&QTcpSocket::stateChanged,this,&GTcpSocket::stateChangedSlot);
    connect(this,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>
            (&QAbstractSocket::error),this,&GTcpSocket::errorSlot);

    this->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
}

const QString &GTcpSocket::getHost() const
{
    return this->host;
}

void GTcpSocket::setHost(const QString &host)
{
    this->host = host;
}

quint16 GTcpSocket::getPort() const
{
    return this->port;
}

void GTcpSocket::setPort(quint16 port)
{
    this->port = port;
}

int GTcpSocket::getConnectionTimeout() const
{
    return this->connectionTimeout;
}

void GTcpSocket::setConnectionTimeout(int timeout)
{
    this->connectionTimeout = timeout;
}

bool GTcpSocket::performConnection()
{
    QString res = QString("Trying to connect to %1, at port %2").arg(this->host).arg(this->port);
    qInfo() << res;
    this->connectToHost(this->host,this->port);

    if (!this->waitForConnected(this->connectionTimeout)){
        qInfo() << "Time done!";
        return -1;
    }

    return 0;
}

void GTcpSocket::readyReadSlot()
{
    QByteArray data = this->readAll();
    QString res = QString("Rx from %1 - %2").arg(host).arg(QString::fromLatin1(data));
    qInfo() << res;
}

void GTcpSocket::connectedSlot()
{
    qInfo() << "Connected";
}

void GTcpSocket::disconnectedSlot()
{
    qInfo() << "Disconnected";
}

void GTcpSocket::errorSlot(QAbstractSocket::SocketError socketError)
{
    qInfo() << "Error" << socketError;
}

void GTcpSocket::stateChangedSlot(QAbstractSocket::SocketState socketState)
{
    qInfo() << "State Changed" << socketState;
}

void GTcpSocket::writeSlot(QByteArray send)
{
    QString res = QString("Tx to %1 - %2").arg(host).arg(QString::fromLatin1(send));
    qInfo() << res;
    this->write(send);
}
