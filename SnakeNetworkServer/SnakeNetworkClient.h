#ifndef SNAKE_NETWORK_CLIENT
#define SNAKE_NETWORK_CLIENT
#include <QObject>

class Snake;
class QString;
class QTcpSocket;

class SnakeNetworkClient : public QObject
{
	Q_OBJECT
public:
	SnakeNetworkClient(QTcpSocket *socket);
	~SnakeNetworkClient();
	Snake *snake;
	QString *username;
signals:
	void connectionClosed();
protected slots:
	void closeConnection();
	void readRequest();
private:
	QTcpSocket *socket;
	unsigned short nextBlockSize;
};

#endif
