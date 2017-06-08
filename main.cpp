#include <CFaceBaseGUI.h>

#include <QtWidgets/QApplication.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	CFaceBaseWindow window(argv[0]);
	window.show();

	return app.exec();
}