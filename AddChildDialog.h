// Class hierarchy
#include <QtWidgets/QDialog>

#include <string>
#include <tuple>

class QLineEdit;

class AddChildDialog : public QDialog
{
public:
	explicit AddChildDialog(QWidget* parent = nullptr);

	using ChildInfo = std::tuple<std::string, std::string, std::string>;
	ChildInfo GetChildInfo() const;

private:
	QLineEdit* nameInput;
	QLineEdit* surnameInput;
	QLineEdit* schoolInput;
};