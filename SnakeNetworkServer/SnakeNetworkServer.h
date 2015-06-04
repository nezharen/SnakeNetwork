#ifndef SNAKE_NETWORK_SERVER
#define SNAKE_NETWORK_SERVER
#include <QObject>

class QTcpServer;
class QTimer;
class Point;
class Snake;
class SnakeNetworkClient;

class SnakeNetworkClientLink
{
public:
	SnakeNetworkClientLink(SnakeNetworkClient *client);
	SnakeNetworkClient *client;
	SnakeNetworkClientLink *next;
};

class SnakeNetworkServer : public QObject
{
	Q_OBJECT
public:
	SnakeNetworkServer();
	Snake* newSnake();
	Point* newFood();
	void juageAlive(Snake *p, Snake *q);
signals:
	void clientUpdate(QByteArray *snapShot);
protected slots:
	void acceptNewConnection();
	void closeConnection();
	void checkUsername();
	void update();
private:
	QTcpServer *serverSocket;
	QTimer *updateTimer;
	SnakeNetworkClientLink *head, *tail;
	unsigned short clientNum;
	QByteArray *snapShot;
	Point *food;
};

#endif

