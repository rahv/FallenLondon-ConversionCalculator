
#ifndef UTILS_H
#define UTILS_H

class QString;
#include <string>
#include <list>

namespace Utils
{
	/**
	 * Displays an error in a QMessageBox
	 * \param e The error message.
	 * \sa QMessageBox
	 */
	void message(const QString &e);

	/**
	 * Displays an error in a QMessageBox
	 * \param e The error message.
	 * \param t The title of the message box
	 * \sa QMessageBox
	 */
	void message(const QString &e, const QString &t);

	/**
	 * Displays a question in a QMessageBox (offering Ok | Cancel options)
	 * Default value is 'Cancel' so that no bad things happen if the user
	 * presses enter without reading the text (e.g. when overwriting files)
	 * \param e The error message.
	 * \param t The title of the message box
	 * \return 'true' if 'Ok' has been pressed, 'false' otherwise
	 * \sa QMessageBox
	 */
	bool question(const QString &e, const QString &t);

	std::list<std::string> splitString(const std::string &str, char delim);

}

#endif //UTILS_H
