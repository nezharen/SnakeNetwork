#ifndef SNAKE_NETWORK_CLIENT_DIALOG
#define SNAKE_NETWORK_CLIENT_DIALOG
#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class SnakeNetworkClientDialog : public QDialog
{
	Q_OBJECT
public:
	SnakeNetworkClientDialog();
	~SnakeNetworkClientDialog();
private:
	QLabel *usernameLabel;
	QLineEdit *usernameEdit;
	QPushButton *okButton, *exitButton;
	QHBoxLayout *usernameLayout, *buttonLayout;
	QVBoxLayout *mainLayout;
};

#endif
