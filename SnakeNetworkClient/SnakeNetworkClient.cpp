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
	length = 0;
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
		socket->close();
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
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << quint16(CMD_USERNAME) << (*username);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	socket->write(block);
}

void SnakeNetworkClient::readResponse()
{
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
		case CMD_ERROR:
			emit getError();
			break;
		case CMD_UPDATE:
			imagePainter.setRenderHint(QPainter::Antialiasing, true);
			imagePainter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
			imagePainter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
			imagePainter.eraseRect(image->rect());
			imagePainter.drawRect(image->rect());
			imagePainter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine, Qt::FlatCap));
			for (int i = 0; i < IMAGE_LENGTH; i += bodyLength)
			{
				imagePainter.drawLine(i, 0, i, IMAGE_LENGTH);
				imagePainter.drawLine(0, i, IMAGE_LENGTH, i);
			}
			imagePainter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
			in >> clientNum;
			for (int i = 0; i < clientNum; i++)
			{
				in >> username;
				in >> length;
				if (length == 0)
					continue;
				imagePainter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
				in >> point.x >> point.y;
				imagePainter.drawEllipse(point.x * bodyLength, point.y * bodyLength, bodyLength, bodyLength);
				if (this->username == username)
				{
					imagePainter.setBrush(QBrush(Qt::darkYellow, Qt::SolidPattern));
					this->length = length;
				}
				else
					imagePainter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
				for (int j = 1; j < length; j++)
				{
					in >> point.x >> point.y;
					imagePainter.drawEllipse(point.x * bodyLength, point.y * bodyLength, bodyLength, bodyLength);
				}
			}
			imagePainter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
			in >> point.x >> point.y;
			imagePainter.drawEllipse(point.x * bodyLength, point.y * bodyLength, bodyLength, bodyLength);
			imagePainter.end();
			emit updateMainWindow();
			break;
		default:
			break;
		}

		nextBlockSize = 0;
	}
}

void SnakeNetworkClient::changeDirection(const unsigned short &cmd)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << quint16(cmd);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	socket->write(block);
}

