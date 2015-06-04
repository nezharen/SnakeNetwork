#ifndef SNAKE_NETWORK_CLIENT
#define SNAKE_NETWORK_CLIENT
#include <QObject>

class Snake;
class QString;
class QTcpSocket;
class QByteArray;

class SnakeNetworkClient : public QObject
{
	Q_OBJECT
public:
	SnakeNetworkClient(QTcpSocket *socket);
	void sendOK();
	void sendError();
	Snake *snake;
	QString *username;
	bool inited;
signals:
	void connectionClosed();
	void checkUsername();
protected slots:
	void closeConnection();
	void readRequest();
	void updateClient(QByteArray *snapShot);
private:
	QTcpSocket *socket;
	unsigned short nextBlockSize;
};

#endif
