#include <QApplication>
#include "SnakeNetworkClientDialog.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	SnakeNetworkClientDialog dialog;

	dialog.show();
	return app.exec();
}

