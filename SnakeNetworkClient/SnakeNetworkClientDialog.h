#ifndef SNAKE_NETWORK_CLIENT_DIALOG
#define SNAKE_NETWORK_CLIENT_DIALOG
#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class SnakeNetworkClient;

class SnakeNetworkClientDialog : public QDialog
{
	Q_OBJECT
public:
	SnakeNetworkClientDialog();
	~SnakeNetworkClientDialog();
protected slots:
	void connectServer();
	void showMainWindow();
private:
	QLabel *serverIpLabel, *usernameLabel;
	QLineEdit *serverIpEdit, *usernameEdit;
	QPushButton *okButton, *exitButton;
	QHBoxLayout *topLayout, *buttonLayout;
	QVBoxLayout *labelLayout, *editLayout, *mainLayout;
	SnakeNetworkClient *client;
};

#endif
