#include "gtcpclient.h"

#include <QDebug>

GTcpClient::GTcpClient(QObject *parent) :
  QTcpSocket(parent),
  host("127.0.0.1"),
  port(1234),
  connectionTimeout(5000)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    connect(this,SIGNAL(connected()),this,SLOT(connectedSlot()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChangedSlot(QAbstractSocket::SocketState)));
}

const QString &GTcpClient::getHost() const
{
    return this->host;
}

void GTcpClient::setHost(const QString &host)
{
    this->host = host;
}

quint16 GTcpClient::getPort() const
{
    return this->port;
}

void GTcpClient::setPort(quint16 port)
{
    this->port = port;
}

int GTcpClient::getConnectionTimeout() const
{
    return this->connectionTimeout;
}

void GTcpClient::setConnectionTimeout(int timeout)
{
    this->connectionTimeout = timeout;
}

bool GTcpClient::performConnection()
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

void GTcpClient::readyReadSlot()
{
    QByteArray data = this->readAll();
    QString res = QString("Rx from %1 - %2").arg(host).arg(QString::fromLatin1(data));
    qInfo() << res;
}

void GTcpClient::connectedSlot()
{
    qInfo() << "Connected";
}

void GTcpClient::disconnectedSlot()
{
    qInfo() << "Disconnected";
}

void GTcpClient::stateChangedSlot(QAbstractSocket::SocketState socketState)
{
    qInfo() << "State Changed" << socketState;
}

void GTcpClient::writeSlot(QByteArray send)
{
    QString res = QString("Tx to %1 - %2").arg(host).arg(QString::fromLatin1(send));
    qInfo() << res;
    this->write(send);
}
