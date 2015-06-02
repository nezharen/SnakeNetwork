#include <QtNetwork>
#include <QString>
#include "Snake.h"
#include "SnakeNetworkClient.h"

SnakeNetworkClient::SnakeNetworkClient(QTcpSocket *socket)
{
	this->snake = NULL;
	this->username = NULL;
	this->nextBlockSize = 0;
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
}

void SnakeNetworkClient::closeConnection()
{
	emit connectionClosed();
}

void SnakeNetworkClient::readRequest()
{
	
}

