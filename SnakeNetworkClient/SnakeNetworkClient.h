#ifndef SNAKE_NETWORK_CLIENT
#define SNAKE_NETWORK_CLIENT
#include <QObject>

class QString;
class QTcpSocket;
class QImage;

class SnakeNetworkClient : public QObject
{
	Q_OBJECT
public:
	SnakeNetworkClient(QString *serverIp, QString *username);
	~SnakeNetworkClient();
	QString *username;
	QImage *image;
	unsigned short length;
signals:
	void getOK();
	void updateMainWindow();
protected slots:
	void sendRequest();
	void readResponse();
	void changeDirection(const unsigned short &cmd);
private:
	QString *serverIp;
	QTcpSocket *socket;
	unsigned short nextBlockSize;
};

#endif

