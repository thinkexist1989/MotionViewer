#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdlg.h"
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ndiComm(new NdiComm),
    ndiViewer(new NdiViewer),
    holoComm(new HoloComm),
    holoViewer(new HoloViewer)
{
    ui->setupUi(this);
    ui->ndiCommDockWidget->setWidget(ndiComm);
    ui->ndiViewerDockWidget->setWidget(ndiViewer);
    ui->holoCommDockWidget->setWidget(holoComm);
    ui->holoViewerDockWidget->setWidget(holoViewer);
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
