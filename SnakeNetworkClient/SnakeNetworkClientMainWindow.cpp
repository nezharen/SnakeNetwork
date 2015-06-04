#include <QtGui>
#include "SnakeNetworkClient.h"
#include "SnakeNetworkClientMainWindow.h"

SnakeNetworkClientMainWindow::SnakeNetworkClientMainWindow(SnakeNetworkClient *client)
{
	this->client = client;
	connect(this->client, SIGNAL(updateMainWindow()), this, SLOT(updateMainWindow()));
	setFixedSize(820, 660);
	setWindowTitle(tr("SnakeNetwork"));
}

SnakeNetworkClientMainWindow::~SnakeNetworkClientMainWindow()
{
	if (client != NULL)
		delete client;
}

void SnakeNetworkClientMainWindow::updateMainWindow()
{
	update();
}

void SnakeNetworkClientMainWindow::paintEvent(QPaintEvent *event)
{
	QPainter windowPainter(this);
	windowPainter.setRenderHint(QPainter::Antialiasing, true);
	windowPainter.drawImage(20, 20, *(client->image));
	windowPainter.drawText(650, 40, tr("Username: %1").arg(*(client->username)));
	windowPainter.drawText(650, 70, tr("Score: %1").arg(client->length));
}

