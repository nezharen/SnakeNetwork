#include <iostream>
#include <QtNetwork>
#include "defs.h"
#include "SnakeNetworkClient.h"
#include "SnakeNetworkServer.h"

SnakeNetworkClientLink::SnakeNetworkClientLink(SnakeNetworkClient *client)
{
	this->client = client;
	next = NULL;
}

SnakeNetworkClientLink::~SnakeNetworkClientLink()
{
	if (client != NULL)
		client->deleteLater();
}

SnakeNetworkServer::SnakeNetworkServer()
{
	serverSocket = new QTcpServer(this);
	connect(serverSocket, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
	if (!(serverSocket->listen(QHostAddress::Any, SERVER_PORT)))
	{
		std::cerr << "Cannot listen port: " << SERVER_PORT << std::endl;
		qApp->quit();
	}
	head = NULL;
	tail = NULL;
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
	if (head != NULL)
	{
		SnakeNetworkClientLink *p;

		while (head != NULL)
		{
			p = head->next;
			delete head;
			head = p;
		}
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
	SnakeNetworkClient *newClient = new SnakeNetworkClient(serverSocket->nextPendingConnection());
	connect(newClient, SIGNAL(connectionClosed()), this, SLOT(closeConnection()));
	if (head == NULL)
	{
		head = new SnakeNetworkClientLink(newClient);
		tail = head;
	}
	else
	{
		tail->next = new SnakeNetworkClientLink(newClient);
		tail = tail->next;
	}
}

void SnakeNetworkServer::closeConnection()
{
	if (SnakeNetworkClient *client = qobject_cast<SnakeNetworkClient *>(sender()))
	{
		std::cout << "closeConnection"  << std::endl;
		SnakeNetworkClientLink *p, *q;
		p = q = head;
		while (p != NULL)
		{
			if (p->client == client)
			{
				if (p == head)
				{
					head = head->next;
					q = head;
				}
				else
					q->next = p->next;
				if (p == tail)
					tail = q;
				delete p;
				break;
			}
			q = p;
			p = p->next;
		}
	}
}

void SnakeNetworkServer::update()
{
	std::cout << "update" << std::endl;
	
}

