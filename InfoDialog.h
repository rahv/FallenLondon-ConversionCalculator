#ifndef INFODIALOG_H
#define INFODIALOG_H

#include "ui_Info.h"
#include <QDialog>

/**
 * \brief A dialog window displaying a bit of background information
 */
class InfoDialog : public QDialog, private Ui_Info
{
	Q_OBJECT

public:
	InfoDialog(QDialog* parent = 0);
	~InfoDialog() {};

private:
	void setText();

private slots:
	void on_okPushButton_pressed();

};

#endif //INFODIALOG_H
