#include <AddChildDialog.h>

#include <QtWidgets\QLineEdit.h>
#include <QtWidgets\QLabel.h>
#include <QtWidgets\QFormLayout>
#include <QtWidgets\QDialogButtonBox.h>

AddChildDialog::AddChildDialog(QWidget* parent /* = nullptr */)
	: QDialog(parent), nameInput(new QLineEdit), surnameInput(new QLineEdit), schoolInput(new QLineEdit)
{
	setMinimumSize(400, 300);

	QFormLayout* layout = new QFormLayout;
	layout->addRow("Child name", nameInput);
	layout->addRow("Child Surname", surnameInput);
	layout->addRow("School", schoolInput);

	QDialogButtonBox* buttons = new QDialogButtonBox;
	buttons->addButton("Okay", QDialogButtonBox::AcceptRole);
	buttons->addButton("Cancel", QDialogButtonBox::RejectRole);
	layout->addWidget(buttons);
	setLayout(layout);

	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

AddChildDialog::ChildInfo AddChildDialog::GetChildInfo() const
{
	return std::make_tuple
	(
		nameInput->displayText().toStdString(),
		surnameInput->displayText().toStdString(),
		schoolInput->displayText().toStdString()
	);
}