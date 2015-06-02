#include <iostream>
#include <QtNetwork>
#include <QTimer>
#include "SnakeNetworkServer.h"
#include "defs.h"

SnakeNetworkServer::SnakeNetworkServer()
{
	serverSocket = new QTcpServer();
	connect(serverSocket, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
	if (!(serverSocket->listen(QHostAddress::Any, SERVER_PORT)))
	{
		std::cerr << "Cannot listen port: " << SERVER_PORT << std::endl;
		qApp->quit();
	}
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	updateTimer->start(1000);
	std::cout << "SnakeNetworkServer start successful." << std::endl;
}

SnakeNetworkServer::~SnakeNetworkServer()
{
	if (updateTimer != NULL)
	{
		updateTimer->stop();
		delete updateTimer;
	}
	if (serverSocket != NULL)
	{
		if ((serverSocket->isListening()))
			serverSocket->close();
		delete serverSocket;
	}
	std::cout << "SnakeNetworkServer closed." << std::endl;
}

void SnakeNetworkServer::acceptNewConnection()
{
	std::cout << "newConnection" << std::endl;

}

void SnakeNetworkServer::update()
{
	std::cout << "update" << std::endl;
	
}
