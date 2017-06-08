/*
* File:   main.cpp
* Author: Tas
*
* Created on 25 March 2016, 7:35 AM
*/

#include <QtWidgets\QApplication.h>
#include <QtWidgets\QMainWindow.h>
#include <QtWidgets\QMenuBar.h>

#include <string>

class QDragEnterEvent;

class CFaceBaseWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit CFaceBaseWindow(std::string exeDirectory);
	~CFaceBaseWindow() override;

	void dragEnterEvent(QDragEnterEvent* pEvent) override;
	void dropEvent(QDropEvent* pEvent) override;

private slots:
	void ShowAddChildMenu();
	void OpenImageMenu();

private:
	static const std::string databaseName;
	const std::string databasePathAndName;
};
