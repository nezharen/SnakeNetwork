#ifndef SNAKE_NETWORK_CLIENT
#define SNAKE_NETWORK_CLIENT
#include <QObject>

class QString;
class QTcpSocket;

class SnakeNetworkClient : public QObject
{
	Q_OBJECT
public:
	SnakeNetworkClient(QString *serverIp, QString *username);
	~SnakeNetworkClient();
signals:
	void getOK();
protected slots:
	void sendRequest();
	void readResponse();
private:
	QString *serverIp, *username;
	QTcpSocket *socket;
	unsigned short nextBlockSize;
};

#endif

