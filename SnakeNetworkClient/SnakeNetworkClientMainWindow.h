#ifndef SNAKE_NETWORK_CLIENT_MAIN_WINDOW_H
#define SNAKE_NETWORK_CLIENT_MAIN_WINDOW_H
#include <QMainWindow>

class SnakeNetworkClient;

class SnakeNetworkClientMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	SnakeNetworkClientMainWindow(SnakeNetworkClient *client);
	~SnakeNetworkClientMainWindow();
protected slots:
	void updateMainWindow();
protected:
	void paintEvent(QPaintEvent *event);
private:
	SnakeNetworkClient *client;
};

#endif

