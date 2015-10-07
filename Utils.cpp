#include "Utils.h"

#include <sstream>
#include <QMessageBox>
#include <QString>

namespace Utils
{

void message(const QString &e)
{
	message(e, "Error");
}

void message(const QString &e, const QString &t)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(t);
	msgBox.setText(e);
	msgBox.exec();
}

bool question(const QString &e, const QString &t)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(t);
	msgBox.setText(e);
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);

	if (msgBox.exec() == QMessageBox::Ok)
		return true;
	return false;
}

std::list<std::string> splitString(const std::string &str, char delim)
{
	std::list<std::string> strList;
	std::stringstream ss(str);
	std::string item;
	while(getline(ss, item, delim))
		strList.push_back(item);
	return strList;
}

}
