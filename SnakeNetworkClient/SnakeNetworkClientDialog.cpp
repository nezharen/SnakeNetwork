#include <QtGui>
#include "SnakeNetworkClientDialog.h"

SnakeNetworkClientDialog::SnakeNetworkClientDialog()
{
	usernameLabel = new QLabel(tr("&Username"));
	usernameEdit = new QLineEdit;
	usernameEdit->setAlignment(Qt::AlignHCenter);
	usernameLabel->setBuddy(usernameEdit);
	okButton = new QPushButton(tr("&OK"));
	okButton->setDefault(true);
	exitButton = new QPushButton(tr("&Exit"));
	connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	usernameLayout = new QHBoxLayout;
	usernameLayout->addWidget(usernameLabel);
	usernameLayout->addWidget(usernameEdit);

	buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(okButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(exitButton);

	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(usernameLayout);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("SnakeNetwork"));
	setFixedSize(sizeHint());
}

SnakeNetworkClientDialog::~SnakeNetworkClientDialog()
{
	delete usernameLabel;
	delete usernameEdit;
	delete okButton;
	delete exitButton;
	delete usernameLayout;
	delete buttonLayout;
	delete mainLayout;
}

