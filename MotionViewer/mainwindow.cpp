#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    setStatusMsg(tr("Open About Dialog"));
    AboutDlg* aboutDlg = new AboutDlg(this);
    aboutDlg->show();
}

void MainWindow::setStatusMsg(QString msg)
{
    ui->statusBar->showMessage(msg,2000);
}
