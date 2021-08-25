#include "gtcpsocket.h"

#include <QHostAddress>

GTcpSocket::GTcpSocket(QObject *parent) : QObject(parent)
{
    connect(&m_timer_reconnection, &QTimer::timeout, this,
            &GTcpSocket::tryConnect);
    m_socket = nullptr;
    m_reconnect = false;
}

GTcpSocket::~GTcpSocket()
{
    m_timer_reconnection.stop();

    disconnect(&m_timer_reconnection, &QTimer::timeout, this,
               &GTcpSocket::tryConnect);

    if (m_socket)
    {
        m_socket->close();
        m_socket->deleteLater();
    }
}

const QString &GTcpSocket::ip() const
{
    return m_ip;
}

void GTcpSocket::setIp(const QString &newIp)
{
    m_ip = newIp;
}

quint16 GTcpSocket::port() const
{
    return m_port;
}

void GTcpSocket::setPort(quint16 newPort)
{
    m_port = newPort;
}

QTcpSocket *GTcpSocket::socket() const
{
    return m_socket;
}

void GTcpSocket::setSocket(QTcpSocket *newSocket)
{
    stop();

    connect(newSocket, &QTcpSocket::readyRead, this, &GTcpSocket::readyRead);
    connect(newSocket, &QTcpSocket::stateChanged, this,
            &GTcpSocket::stateChanged);
    connect(newSocket, &QTcpSocket::disconnected,
            [this]() { emit disconnected(); });

    m_socket = newSocket;
}

void GTcpSocket::tryConnect()
{
    if (!m_socket)
    {
        QTcpSocket *_socket = new QTcpSocket(this);
        setSocket(_socket);
    }

    if (!m_reconnect)
        return;

    if (!m_socket->isOpen())
        m_socket->connectToHost(m_ip, m_port);
}

void GTcpSocket::readyRead() // Hacer configurables los validadores
{
    QByteArray data;
    QString hostAddress = m_socket->peerAddress().toString();

    data = m_socket->readAll();

    qInfo() << QString("%1: Rx from %2 - %3")
                   .arg(objectName(), hostAddress, QString::fromLatin1(data));

    emit dataReceived(data);
}

void GTcpSocket::start()
{
    tryConnect();
    m_timer_reconnection.start(5000);
}

void GTcpSocket::stop()
{
    if (m_socket)
    {
        disconnect(m_socket, &QTcpSocket::readyRead, this,
                   &GTcpSocket::readyRead);
        disconnect(m_socket, &QTcpSocket::stateChanged, this,
                   &GTcpSocket::stateChanged);
        disconnect(m_socket, &QTcpSocket::disconnected, this,
                   &GTcpSocket::disconnected);

        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    m_timer_reconnection.stop();
}

void GTcpSocket::serverSettings(QString ipx, int portx)
{
    m_ip = ipx;
    m_port = portx;
}

void GTcpSocket::send(QByteArray data)
{
    if (!m_socket)
        return;

    QString logdata;
    QString hostAddress = m_socket->peerAddress().toString();

    logdata = QString("%1: Tx to %2 - %3")
                  .arg(objectName())
                  .arg(hostAddress)
                  .arg(QString::fromLatin1(data));
    qInfo() << logdata;

    m_socket->write(data);
}

void GTcpSocket::stateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
    case QAbstractSocket::UnconnectedState:
        qDebug() << QString("%1: UnconnectedState").arg(objectName());
        stop();

        if (m_reconnect)
            m_timer_reconnection.start(5000);

        emit disconnected();
        break;
    case QAbstractSocket::HostLookupState:
        qDebug() << QString("%1: HostLookupState").arg(objectName());
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << QString("%1: ConnectingState").arg(objectName());
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << QString("%1: Conectado a %2:%3")
                        .arg(objectName())
                        .arg(m_ip)
                        .arg(m_port);
        break;
    case QAbstractSocket::BoundState:
        qDebug() << QString("%1: BoundState").arg(objectName());
        break;
    case QAbstractSocket::ListeningState:
        qDebug() << QString("%1: ListeningState").arg(objectName());
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << QString("%1: ClosingState").arg(objectName());
        break;
    }
}

void GTcpSocket::error(QAbstractSocket::SocketError socketError)
{
}

bool GTcpSocket::reconnect() const
{
    return m_reconnect;
}

void GTcpSocket::setReconnect(bool newReconnect)
{
    m_reconnect = newReconnect;
}
