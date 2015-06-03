#include <iostream>
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

SnakeNetworkClientDialog::~SnakeNetworkClientDialog()
{
	delete serverIpLabel;
	delete serverIpEdit;
	delete usernameLabel;
	delete usernameEdit;
	delete okButton;
	delete exitButton;
	delete labelLayout;
	delete editLayout;
	delete topLayout;
	delete buttonLayout;
	delete mainLayout;
}

void SnakeNetworkClientDialog::connectServer()
{
	std::cout << "Connecting server." << std::endl;
	okButton->setEnabled(false);
	okButton->setText(tr("&Connecting"));
	client = new SnakeNetworkClient(new QString(serverIpEdit->text()), new QString(usernameEdit->text()));
	connect(client, SIGNAL(getOK()), this, SLOT(showMainWindow()));
}

void SnakeNetworkClientDialog::showMainWindow()
{
	SnakeNetworkClientMainWindow *mainWindow = new SnakeNetworkClientMainWindow(client);
	mainWindow->show();
	mainWindow->raise();
	mainWindow->activateWindow();
	close();
}

