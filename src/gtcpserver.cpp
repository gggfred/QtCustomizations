#include "gtcpserver.h"

GTcpServer::GTcpServer(QObject *parent) : QTcpServer(parent), m_port(1234)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

GTcpServer::~GTcpServer()
{
    QTcpServer::close();
}

quint16 GTcpServer::port() const
{
    return m_port;
}

void GTcpServer::setPort(quint16 newPort)
{
    m_port = newPort;
}

bool GTcpServer::performListening()
{
    if (!this->listen(QHostAddress::Any, this->m_port))
    {
        qCritical() << "Unable to start Server" << this->errorString();
        return -1;
    }
    else
    {
        qInfo() << QString("%1: Listening on port %2")
                       .arg(objectName(), QString::number(this->m_port));
    }
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.length(); i++)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            //            qInfo() << ipAddressesList.at(i).toString();
        }
    }
    return 0;
}

void GTcpServer::stop()
{
    close();
}

void GTcpServer::newConnection()
{
    QTcpSocket *socket;

    socket = this->nextPendingConnection();

    if (!socket)
        return;

    QString res = QString("%1: Client connected from %2")
                      .arg(objectName())
                      .arg(socket->peerAddress().toString());
    qInfo() << res;

    emit clientConnected(socket);
}