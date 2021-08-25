#ifndef GTCPSOCKET_H
#define GTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class GTcpSocket : public QObject
{
    Q_OBJECT

  public:
    explicit GTcpSocket(QObject *parent = nullptr);
    ~GTcpSocket();

    const QString &ip() const;
    void setIp(const QString &newIp);

    quint16 port() const;
    void setPort(quint16 newPort);

    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *newSocket);

    bool reconnect() const;
    void setReconnect(bool newReconnect);

  signals:
    void dataReceived(QByteArray);
    void disconnected();

  public slots:
    void start();
    void stop();
    void readyRead();
    void tryConnect();
    void serverSettings(QString ip, int port);
    void send(QByteArray data);

  private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void error(QAbstractSocket::SocketError socketError);

  private:
    QString m_ip;
    quint16 m_port;
    bool m_reconnect;
    QTcpSocket *m_socket;
    QTimer m_timer_reconnection;
};

#endif // GTCPSOCKET_H
