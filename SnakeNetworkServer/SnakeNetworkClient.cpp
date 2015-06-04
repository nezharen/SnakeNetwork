#include <iostream>
#include <QtNetwork>
#include <QString>
#include "defs.h"
#include "Snake.h"
#include "SnakeNetworkClient.h"

SnakeNetworkClient::SnakeNetworkClient(QTcpSocket *socket)
{
	snake = NULL;
	username = NULL;
	inited = false;
	nextBlockSize = 0;
	this->socket = socket;
	connect(this->socket, SIGNAL(disconnected()), this, SLOT(closeConnection()));
	connect(this->socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
}

SnakeNetworkClient::~SnakeNetworkClient()
{
	if (username != NULL)
		delete username;
	if (socket != NULL)
	{
		socket->close();
		delete socket;
	}
	if (snake != NULL)
		delete snake;
	std::cout << "Connection closed." << std::endl;
}

void SnakeNetworkClient::closeConnection()
{
	emit connectionClosed();
}

void SnakeNetworkClient::readRequest()
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
		case CMD_USERNAME:
			if (username == NULL)
			{
				username = new QString;
			}
			in >> (*username);
			break;
		default:
			break;
		}

		nextBlockSize = 0;
	}
}

void SnakeNetworkClient::updateClient(QByteArray *snapShot)
{
	std::cout << "updateClient" << std::endl;
	socket->write(*snapShot);
}

void SnakeNetworkClient::sendOK()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) <<  quint16(CMD_OK);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	socket->write(block);
	std::cout << "OK sent." << std::endl;
}

