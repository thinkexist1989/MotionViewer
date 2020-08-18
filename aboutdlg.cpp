#include "aboutdlg.h"
AboutDlg::AboutDlg(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    setFixedSize(430,260);
    //setFixedSize(this->sizeHint());
    //setWindowFlag(Qt::FramelessWindowHint);
}

AboutDlg::~AboutDlg()
{
}
