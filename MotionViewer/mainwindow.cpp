#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdlg.h"
#include <QDockWidget>
#include <QDebug>

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

    //connect(ndiComm, &NdiComm::initFinished, this, [=](QString msg){qDebug() << msg;});
    //connect(ndiComm, &NdiComm::dataReady, this, [=](QList<QVector3D> markers){ qDebug() << markers; });
    //connect(holoComm, &HoloComm::dataReady, this, [=](QString data){qDebug() << "Received data is: " << data;});


    connect(ndiComm, &NdiComm::dataReady, ndiViewer, &NdiViewer::dataProc);
    connect(holoComm, &HoloComm::dataReady,holoViewer, &HoloViewer::dataProc);
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

void MainWindow::on_actionNdiViewer_toggled(bool arg1)
{
    if(!arg1) {
        ui->ndiCommDockWidget->close();
        ui->ndiViewerDockWidget->close();
    }
    else {
        ui->ndiCommDockWidget->show();
        ui->ndiViewerDockWidget->show();
    }
}

void MainWindow::on_actionHoloViewer_toggled(bool arg1)
{
    if(!arg1) {
        ui->holoCommDockWidget->close();
        ui->holoViewerDockWidget->close();
    }
    else {
        ui->holoCommDockWidget->show();
        ui->holoViewerDockWidget->show();
    }
}
