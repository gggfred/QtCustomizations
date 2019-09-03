#include "gtcpserver.h"

#include <QNetworkInterface>
#include <QDebug>

GTcpServer::GTcpServer(QObject *parent) :
	QTcpServer (parent),
	port(1234)
{
	connect(this,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

void GTcpServer::setPort(quint16 port)
{
	this->port = port;
}

quint16 GTcpServer::getPort() const
{
	return this->port;
}

bool GTcpServer::performListening()
{
	if (!this->listen(QHostAddress::Any,this->port)){
		qCritical() << "Unable to start Server" << this->errorString();
		return -1;
	}else{
		qInfo() << "Server listening on: ";
		qInfo() << "Address" << this->serverAddress().toIPv4Address() << "Port" << this->serverPort();
	}

	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for(int i=0; i < ipAddressesList.length(); i++) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()){
			qInfo() << ipAddressesList.at(i).toString();
		}
	}
	return 0;
}

QByteArray GTcpServer::proccess(QByteArray data)
{
	QByteArray res;
	if(data.isEmpty())
		res = "Empty string\r\n";
	else
		res = "You ask, I answer!\r\n";

	return res;
}

void GTcpServer::newConnection()
{
	QTcpSocket *socket = this->nextPendingConnection();
	QString res = QString("Client connected on: %1 : %2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
	qInfo() << res;
	connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
	connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
	connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
	connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorReceived(QAbstractSocket::SocketError)));
}

void GTcpServer::disconnected()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
	qInfo() << "Client disconnected";
	socket->deleteLater();
}

void GTcpServer::readyRead()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
	QByteArray answer;
	QString res;

	//  Read packet here
	QByteArray data = socket->readAll();
	res = QString("Rx from %1 - %2").arg(socket->peerAddress().toString()).arg(QString::fromLatin1(data));
	qInfo() << res;

	//  Process the packet here
	answer = proccess(data);

	//  Prepare the answer here
	if(!answer.isEmpty()){
		res = QString("Tx to %1 - %2").arg(socket->peerAddress().toString()).arg(QString::fromLatin1(answer));
		qInfo() << res;

		socket->write(answer);
		socket->flush();
	}
}

void GTcpServer::stateChanged(QAbstractSocket::SocketState socketState)
{
	qInfo() << "State Changed" << socketState;
}

void GTcpServer::errorReceived(QAbstractSocket::SocketError socketError)
{
	qInfo() << "Error" << socketError;
}
