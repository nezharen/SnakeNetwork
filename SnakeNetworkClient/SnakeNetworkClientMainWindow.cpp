#include <QtGui>
#include "defs.h"
#include "SnakeNetworkClient.h"
#include "SnakeNetworkClientMainWindow.h"

SnakeNetworkClientMainWindow::SnakeNetworkClientMainWindow(SnakeNetworkClient *client)
{
	this->client = client;
	connect(this->client, SIGNAL(updateMainWindow()), this, SLOT(updateMainWindow()));
	connect(this, SIGNAL(changeDirection(const unsigned short &)), this->client, SLOT(changeDirection(const unsigned short &)));
	setFixedSize(820, 660);
	setWindowTitle(tr("SnakeNetwork"));
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

void SnakeNetworkClientMainWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Up:
	case Qt::Key_W:
		emit changeDirection(CMD_UP);
		break;
	case Qt::Key_Down:
	case Qt::Key_S:
		emit changeDirection(CMD_DOWN);
		break;
	case Qt::Key_Left:
	case Qt::Key_A:
		emit changeDirection(CMD_LEFT);
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
		emit changeDirection(CMD_RIGHT);
		break;
	default:
		break;
	}
}

