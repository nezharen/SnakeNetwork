#include <iostream>
#include <QtNetwork>
#include "defs.h"
#include "Point.h"
#include "Snake.h"
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
	clientNum = 0;
	snapShot = NULL;
	srand(time(NULL));
	food = newFood();
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
	if (food != NULL)
		delete food;
	if (snapShot != NULL)
		delete snapShot;
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
	connect(this, SIGNAL(clientUpdate(QByteArray *)), newClient, SLOT(updateClient(QByteArray *)));
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
	clientNum++;
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
				clientNum--;
				break;
			}
			q = p;
			p = p->next;
		}
	}
}

Point* SnakeNetworkServer::newFood()
{
	bool map[MAP_LENGTH][MAP_LENGTH];
	unsigned short sum = MAP_LENGTH * MAP_LENGTH;

	for (unsigned char i = 0; i < MAP_LENGTH; i++)
		for (unsigned char j = 0; j < MAP_LENGTH; j++)
			map[i][j] = true;

	SnakeNetworkClientLink *p = head;
	SnakeBody *q;

	while (p != NULL)
	{
		if (p->client->snake != NULL)
		{
			q = p->client->snake->head;
			while (q != NULL)
			{
				map[q->point->x][q->point->y] = false;
				sum--;
				q = q->next;
			}
		}
		p = p->next;
	}

	if (sum == 0)
		return new Point(MAP_LENGTH, MAP_LENGTH);

	unsigned short tx, ty;
	while (true)
	{
		tx = rand() % MAP_LENGTH;
		ty = rand() % MAP_LENGTH;
		if (map[tx][ty])
			return new Point(tx, ty);
	}
}

Snake* SnakeNetworkServer::newSnake()
{
	bool map[MAP_LENGTH][MAP_LENGTH];

	for (unsigned char i = 0; i < MAP_LENGTH; i++)
		for (unsigned char j = 0; j < MAP_LENGTH; j++)
			map[i][j] = true;

	SnakeNetworkClientLink *p = head;
	SnakeBody *q;

	while (p != NULL)
	{
		if (p->client->snake != NULL)
		{
			q = p->client->snake->head;
			while (q != NULL)
			{
				map[q->point->x][q->point->y] = false;
				q = q->next;
			}
		}
		p = p->next;
	}

	for (unsigned char i = 0; i + 2 < MAP_LENGTH; i++)
		for (unsigned char j = 0; j < MAP_LENGTH; j++)
			if (map[i][j] && map[i + 1][j] && map[i + 2][j])
				return new Snake(new SnakeBody(new Point(i + 1, j)), new SnakeBody(new Point(i, j)), directionRight);
	return NULL;
}

void SnakeNetworkServer::juageAlive(Snake *x, Snake *y)
{
	SnakeBody *p = y->head;
	while (p != NULL)
	{
		if (((x->head->point->x) == (p->point->x)) && ((x->head->point->y) == (p->point->y)))
			x->alive = false;
		p = p->next;
	}
}

void SnakeNetworkServer::update()
{
	SnakeNetworkClientLink *p, *q;
	bool eaten = false;

	p = head;
	while (p != NULL)
	{
		if (p->client->snake != NULL)
		{
			if (p->client->snake->forward(food))
				eaten = true;
			if (((p->client->snake->head->point->x) >= MAP_LENGTH) || ((p->client->snake->head->point->y) >= MAP_LENGTH))
				p->client->snake->alive = false;
		}
		p = p->next;
	}

	p = head;
	while (p != NULL)
	{
		if (p->client->snake != NULL)
		{
			q = p->next;
			while (q != NULL)
			{
				if (q->client->snake != NULL)
				{
					juageAlive(p->client->snake, q->client->snake);
					juageAlive(q->client->snake, p->client->snake);
				}
				q = q->next;
			}
		}
		p = p->next;
	}

	p = head;
	while (p != NULL)
	{
		if (p->client->snake != NULL)
			if (!(p->client->snake->alive))
			{
				delete p->client->snake;
				p->client->snake = NULL;
			}
		p = p->next;
	}

	p = head;
	while (p != NULL)
	{
		if ((p->client->snake == NULL) && (p->client->username != NULL))
		{
			p->client->snake = newSnake();
			if ((!(p->client->inited)) && (p->client->snake != NULL))
			{
				p->client->inited = true;
				p->client->sendOK();
			}
		}
		p = p->next;
	}

	if (eaten)
	{
		delete food;
		food = newFood();
	}

	if (snapShot != NULL)
	{
		delete snapShot;
		snapShot = new QByteArray;
	}
	QDataStream out(snapShot, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << quint16(CMD_UPDATE) << quint16(clientNum);
	SnakeBody *r;
	p = head;
	while (p != NULL)
	{
		out << p->client->username;
		out << quint16(p->client->snake->length);
		r = p->client->snake->head;
		while (r != NULL)
		{
			out << quint8(r->point->x) << quint8(r->point->y);
			r = r->next;
		}
		p = p->next;
	}
	out << quint8(food->x) << quint8(food->y);
	out.device()->seek(0);
	out << quint16(snapShot->size() - sizeof(quint16));
	emit clientUpdate(snapShot);
}

