#include <QCoreApplication>
#include "SnakeNetworkServer.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	SnakeNetworkServer* server = new SnakeNetworkServer();

	return app.exec();
}

