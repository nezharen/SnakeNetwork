#include <iostream>
#include <QtNetwork>
#include "defs.h"
#include "SnakeNetworkClient.h"

SnakeNetworkClient::SnakeNetworkClient(QString *serverIp, QString *username)
{
	this->serverIp = serverIp;
	this->username = username;
	nextBlockSize = 0;
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(connected()), this, SLOT(sendRequest()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(readResponse()));
	socket->connectToHost(*(this->serverIp), SERVER_PORT);
}

SnakeNetworkClient::~SnakeNetworkClient()
{
	if (socket != NULL)
	{
		socket->disconnectFromHost();
		socket->waitForDisconnected();
		delete socket;
	}
	if (username != NULL)
		delete username;
	if (serverIp != NULL)
		delete serverIp;
}

void SnakeNetworkClient::sendRequest()
{
	std::cout << "Sending request." << std::endl;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << quint16(CMD_USERNAME) << (*username);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	socket->write(block);
	std::cout << "Request sended." << std::endl;
}

void SnakeNetworkClient::readResponse()
{
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_3);
	forever
	{
		if (nextBlockSize == 0)
		{
			if (socket->bytesAvailable() < sizeof(quint16))
				break;
			in >> nextBlockSize;
		}

		if (socket->bytesAvailable() < nextBlockSize)
			break;

		quint16 cmd;

		in >> cmd;
		switch (cmd)
		{
		case CMD_OK:
			emit getOK();
			break;
		case CMD_UPDATE:

			break;
		default:
			break;
		}

		nextBlockSize = 0;
	}
}

