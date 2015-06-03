#include <QtGui>
#include "SnakeNetworkClient.h"
#include "SnakeNetworkClientMainWindow.h"

SnakeNetworkClientMainWindow::SnakeNetworkClientMainWindow(SnakeNetworkClient *client)
{
	this->client = client;
	setFixedSize(820, 660);
	setWindowTitle(tr("SnakeNetwork"));
}

SnakeNetworkClientMainWindow::~SnakeNetworkClientMainWindow()
{
	if (client != NULL)
		delete client;
}

