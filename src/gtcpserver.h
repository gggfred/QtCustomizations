#ifndef GTCPSERVER_H
#define GTCPSERVER_H

#include <QDebug>
#include <QNetworkInterface>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class GTcpServer : public QTcpServer
{
    Q_OBJECT
  public:
    explicit GTcpServer(QObject *parent = nullptr);
    ~GTcpServer();

    quint16 port() const;
    void setPort(quint16 newPort);

  public slots:
    bool performListening();
    void stop();

  private slots:
    void newConnection();

  signals:
    void clientConnected(QTcpSocket *socket);

  protected:
    quint16 m_port;
};

#endif // tcpServer_H
