#include <iostream>
#include <QtGui>
#include <QtNetwork>
#include "defs.h"
#include "Point.h"
#include "SnakeNetworkClient.h"

SnakeNetworkClient::SnakeNetworkClient(QString *serverIp, QString *username)
{
	this->serverIp = serverIp;
	this->username = username;
	image = new QImage(IMAGE_LENGTH, IMAGE_LENGTH, QImage::Format_ARGB32_Premultiplied);
	nextBlockSize = 0;
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(connected()), this, SLOT(sendRequest()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(readResponse()));
	socket->connectToHost(*(this->serverIp), SERVER_PORT);
}

SnakeNetworkClient::~SnakeNetworkClient()
{
	if (socket != NULL)
	{
		socket->disconnectFromHost();
		socket->waitForDisconnected();
		delete socket;
	}
	if (image != NULL)
		delete image;
	if (username != NULL)
		delete username;
	if (serverIp != NULL)
		delete serverIp;
}

void SnakeNetworkClient::sendRequest()
{
	std::cout << "Sending request." << std::endl;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << quint16(CMD_USERNAME) << (*username);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	socket->write(block);
	std::cout << "Request sended." << std::endl;
}

void SnakeNetworkClient::readResponse()
{
	std::cout << "readResponse" << std::endl;
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_3);
	forever
	{
		if (nextBlockSize == 0)
		{
			if (socket->bytesAvailable() < sizeof(quint16))
				break;
			in >> nextBlockSize;
		}

		if (socket->bytesAvailable() < nextBlockSize)
			break;

		quint16 cmd;
		quint16 clientNum, length;
		QPainter imagePainter(image);
		QString username;
		Point point(0, 0);
		unsigned short bodyLength = IMAGE_LENGTH / MAP_LENGTH;
		in >> cmd;
		switch (cmd)
		{
		case CMD_OK:
			emit getOK();
			break;
		case CMD_UPDATE:
			std::cout << "Update." << std::endl;
			imagePainter.setRenderHint(QPainter::Antialiasing, true);
			imagePainter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
			imagePainter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
			imagePainter.eraseRect(image->rect());
			imagePainter.drawRect(image->rect());
			imagePainter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
			in >> clientNum;
			std::cout << clientNum << std::endl;
			for (int i = 0; i < clientNum; i++)
			{
				in >> username;
				std::cout << username.toAscii().constData() << std::endl;
				in >> length;
				std::cout << length << std::endl;
				for (int j = 0; j < length; j++)
				{
					in >> point.x >> point.y;
					std::cout << (unsigned int)point.x << " " << (unsigned int)point.y << std::endl;
					imagePainter.drawEllipse(point.x * bodyLength, point.y * bodyLength, bodyLength, bodyLength);
				}
			}
			imagePainter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
			in >> point.x >> point.y;
			std::cout << (unsigned int)point.x << " " << (unsigned int)point.y << std::endl;
			imagePainter.drawRect(point.x * bodyLength, point.y * bodyLength, bodyLength, bodyLength);
			imagePainter.end();
			emit updateMainWindow();
			break;
		default:
			break;
		}

		nextBlockSize = 0;
	}
}

