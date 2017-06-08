// Class declaration
#include <CFaceBaseGUI.h>

#include <AddChildDialog.h>

// SQL
#include <sqlite3.h>

// Qt includes:
#include <QTGui\QDragEnterEvent>
#include <QTCore\QMimeData>
#include <QTWidgets\QLabel>
#include <QtWidgets\QFileDialog.h>

// Standard includes:
#include <iostream>
#include <sstream>

const std::string CFaceBaseWindow::databaseName = "familiarfaces.db";

void CreateDatabase(const std::string& databaseNameAndPath)
{
	sqlite3* database;
	sqlite3_open(databaseNameAndPath.c_str(), &database);
	
	// Do we need a table for schools?
	// Do we need a table for children and locations of their pictures?

	std::ostringstream statement;
	statement << "CREATE TABLE IF NOT EXISTS Children("
		<< "ID int PRIMARY KEY, "
		<< "Name VARCHAR(255) NOT NULL, "
		<< "Surname VARCHAR(255) NOT NULL, "
		<< "School VARCHAR(255));";
	
	char* error;
	if (sqlite3_exec(database, statement.str().c_str(), nullptr, nullptr, &error) != SQLITE_OK)
	{
		std::cout << error << std::endl;
		sqlite3_free(error);
	}

	sqlite3_close(database);
}

CFaceBaseWindow::CFaceBaseWindow(std::string exeDirectory)
	: QMainWindow(),
	databasePathAndName(exeDirectory.substr(0, exeDirectory.find_last_of('\\')) + '\\' + databaseName)
{
	// Add a minimise and close button, and leave maximise disabled
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	setMinimumSize(QSize(800, 600));
	show();

	// Add a file menu
	if (QMenu* const pFileMenu = menuBar()->addMenu("&File"))
	{
		// Add a QMenu if you wish to add more menu options (e.g. Add->Child, Add->School), otherwise use
		// the QAction
		QAction* addChildMenu = pFileMenu->addAction("Add");
		connect(addChildMenu, SIGNAL(triggered()), SLOT(ShowAddChildMenu()));

		QAction* openImageMenu = pFileMenu->addAction("Open");
		connect(openImageMenu, SIGNAL(triggered()), SLOT(OpenImageMenu()));
	}
	// Add an edit menu
	if (QMenu* const pEditMenu = menuBar()->addMenu("&Edit"))
	{
		pEditMenu->addMenu("Change save location");
	}

	// We accept drag-and-drops!
	setAcceptDrops(true);

	CreateDatabase(databasePathAndName);
}

CFaceBaseWindow::~CFaceBaseWindow()
{

}

void CFaceBaseWindow::dragEnterEvent(QDragEnterEvent* pEvent)
{
	// We accept images dragged from Explorer only:
	if (pEvent->mimeData()->hasUrls())
	{
		std::cout << "Event has image data" << std::endl;
		pEvent->acceptProposedAction();
	}
}

void CFaceBaseWindow::dropEvent(QDropEvent* pEvent)
{
	if (pEvent->mimeData()->hasUrls())
	{
		// Retrieve the image URLs from the data
		QList<QUrl> images = pEvent->mimeData()->urls();
		if (images.empty())
		{
			return;
		}

		// Iterate through all the images
		for (auto s : images)
		{
			// Open up each image
			// QPixmap and QImage both seem to read PNG files, but require a plugin for jpg files (found in the
			// imageformats folder)
			// Consider a QImageReader instead?
			QPixmap file;
			std::cout << "Filename: " << s.toLocalFile().toStdString() << std::endl;
			// Retrieve the local file name
			if (!file.load(s.toLocalFile()))
			{
				std::cout << "Cannot load image: " << s.toLocalFile().toStdString() << std::endl;
				continue;
			}

			static QLabel myLabel;
			myLabel.setPixmap(file);

			myLabel.show();
		}
	}
}

void CFaceBaseWindow::ShowAddChildMenu()
{
	AddChildDialog addChild(this);
	if (addChild.exec() == QDialog::Accepted)
	{
		const auto child = addChild.GetChildInfo();
		std::cout << "Child info: \n\t" << std::get<0>(child) << "\n\t" << std::get<1>(child) << "\n\t" << std::get<2>(child) << std::endl;

		// DON'T FORGET TO PURGE INPUTS FROM USERS! Don't want any fuckin' Bobby DROP_TABLES

		sqlite3* database;
		sqlite3_open(databasePathAndName.c_str(), &database);

		std::ostringstream statement;
		statement << "INSERT INTO Children VALUES(null, '"
				  << std::get<0>(child) << "', '"
				  << std::get<1>(child) << "', '"
				  << std::get<2>(child) << "');";
		char* error;
		if (sqlite3_exec(database, statement.str().c_str(), nullptr, nullptr, &error) != SQLITE_OK)
		{
			std::cout << error << std::endl;
			sqlite3_free(error);
		}

		sqlite3_close(database);
	}
}

void CFaceBaseWindow::OpenImageMenu()
{
	auto files = QFileDialog::getOpenFileNames(this, "Select images", "/", "Image Files (*.png *.jpg)");
	for (auto file : files)
		std::cout << file.toStdString() << std::endl;
}
