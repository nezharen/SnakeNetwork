#ifndef SNAKE_NETWORK_SERVER
#define SNAKE_NETWORK_SERVER
#include <QObject>

class QTcpServer;
class QTimer;

class SnakeNetworkServer : QObject
{
	Q_OBJECT
public:
	SnakeNetworkServer();
	~SnakeNetworkServer();
protected slots:
	void acceptNewConnection();
	void update();
private:
	QTcpServer *serverSocket;
	QTimer *updateTimer;
};

#endif
