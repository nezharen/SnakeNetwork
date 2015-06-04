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
	QImage *image;
signals:
	void getOK();
	void updateMainWindow();
protected slots:
	void sendRequest();
	void readResponse();
private:
	QString *serverIp, *username;
	QTcpSocket *socket;
	unsigned short nextBlockSize;
};

#endif

