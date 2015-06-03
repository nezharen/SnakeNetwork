#include <iostream>
#include <QtNetwork>
#include "defs.h"
#include "SnakeNetworkClient.h"

SnakeNetworkClient::SnakeNetworkClient(QString *serverIp, QString *username)
{
	this->serverIp = serverIp;
	this->username = username;
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(connected()), this, SLOT(sendRequest()));
	socket->connectToHost(*(this->serverIp), SERVER_PORT);
}

SnakeNetworkClient::~SnakeNetworkClient()
{
	if (socket != NULL)
	{
		socket->close();
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
}

