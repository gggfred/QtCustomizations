#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class GTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit GTcpSocket(QObject *parent = nullptr);

    const QString& getHost() const;
    void setHost(const QString &host);

    quint16 getPort() const;
    void setPort(quint16 port);

    int getConnectionTimeout() const;
    void setConnectionTimeout(int timeout);

    bool performConnection();

signals:

public slots:
    void readyReadSlot();
    void connectedSlot();
    void disconnectedSlot();
    void errorSlot(QAbstractSocket::SocketError socketError);
    void stateChangedSlot(QAbstractSocket::SocketState socketState);
    void writeSlot(QByteArray send);

protected:
    QString host;
    quint16 port;

    int connectionTimeout;

private:
};

#endif // MYTCPCLIENT_H
