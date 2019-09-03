#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class GTcpServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit GTcpServer(QObject *parent = nullptr);

	void setPort(quint16 port);
	quint16 getPort() const;

	bool performListening();

	virtual QByteArray proccess(QByteArray data);

signals:

public slots:
	void newConnection();
	void disconnected();
	void readyRead();
	void stateChanged(QAbstractSocket::SocketState socketState);
	void errorReceived(QAbstractSocket::SocketError socketError);

protected:
	quint16 port;

};

#endif // MYTCPSERVER_H
