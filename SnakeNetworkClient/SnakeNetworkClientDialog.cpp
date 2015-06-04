#include <QtGui>
#include "SnakeNetworkClient.h"
#include "SnakeNetworkClientDialog.h"
#include "SnakeNetworkClientMainWindow.h"

SnakeNetworkClientDialog::SnakeNetworkClientDialog()
{
	client = NULL;

	serverIpLabel = new QLabel(tr("&Server IP:"));
	serverIpEdit = new QLineEdit;
	serverIpEdit->setAlignment(Qt::AlignHCenter);
	serverIpLabel->setBuddy(serverIpEdit);

	usernameLabel = new QLabel(tr("&Username:"));
	usernameEdit = new QLineEdit;
	usernameEdit->setAlignment(Qt::AlignHCenter);
	usernameLabel->setBuddy(usernameEdit);

	okButton = new QPushButton(tr("&OK"));
	okButton->setDefault(true);
	connect(okButton, SIGNAL(clicked()), this, SLOT(connectServer()));
	exitButton = new QPushButton(tr("&Exit"));
	connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	labelLayout = new QVBoxLayout;
	labelLayout->addWidget(serverIpLabel);
	labelLayout->addWidget(usernameLabel);

	editLayout = new QVBoxLayout;
	editLayout->addWidget(serverIpEdit);
	editLayout->addWidget(usernameEdit);

	topLayout = new QHBoxLayout;
	topLayout->addLayout(labelLayout);
	topLayout->addLayout(editLayout);

	buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(okButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(exitButton);

	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("SnakeNetwork"));
	setFixedSize(sizeHint());
}

void SnakeNetworkClientDialog::connectServer()
{
	okButton->setEnabled(false);
	okButton->setText(tr("&Connecting"));
	client = new SnakeNetworkClient(new QString(serverIpEdit->text()), new QString(usernameEdit->text()));
	connect(client, SIGNAL(getOK()), this, SLOT(showMainWindow()));
	connect(client, SIGNAL(getError()), this, SLOT(handleError()));
}

void SnakeNetworkClientDialog::showMainWindow()
{
	SnakeNetworkClientMainWindow *mainWindow = new SnakeNetworkClientMainWindow(client);
	mainWindow->show();
	mainWindow->raise();
	mainWindow->activateWindow();
	close();
}

void SnakeNetworkClientDialog::handleError()
{
	QMessageBox::critical(this, tr("SnakeNetwork"), tr("Error: username already exists."));
	okButton->setText(tr("&OK"));
	okButton->setEnabled(true);
	client = NULL;
}

