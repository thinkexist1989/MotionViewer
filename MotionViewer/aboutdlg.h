#pragma once

#include <QDialog>
#include "ui_aboutdlg.h"

class AboutDlg : public QDialog
{
	Q_OBJECT

public:
	AboutDlg(QWidget *parent = Q_NULLPTR);
	~AboutDlg();

private:
	Ui::AboutDlg ui;
};
