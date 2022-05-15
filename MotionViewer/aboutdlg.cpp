#include "aboutdlg.h"

/**
 * @brief AboutDlg::AboutDlg
 * @param parent
 * 本项目的作者的联系方式页面
 */

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
