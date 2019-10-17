#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdlg.h"
#include <QDockWidget>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ndiComm(new NdiComm),
    ndiViewer(new NdiViewer),
    holoComm(new HoloComm),
    holoViewer(new HoloViewer)
{
    ui->setupUi(this);
    loadSettings();
    chineseTranslator = new QTranslator(this);
    chineseTranslator->load(":/translations/translation_zh.qm");

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

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat, this);
    ui->actionNdiViewer->setChecked(settings.value("NdiViewer",true).toBool());
    ui->actionHoloViewer->setChecked(settings.value("HoloViewer",true).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat, this);
    settings.setValue("NdiViewer", ui->actionNdiViewer->isChecked());
    settings.setValue("HoloViewer", ui->actionHoloViewer->isChecked());
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QMainWindow::changeEvent(event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, tr("Exit"), tr("Are you sure to close this program?"), QMessageBox::Yes, QMessageBox::No);
    if(result == QMessageBox::Yes) {
        saveSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
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

void MainWindow::on_actionChinese_triggered()
{
    setStatusMsg(tr("GUI Language Changed"));
    qApp->installTranslator(chineseTranslator);
}

void MainWindow::on_actionEnglish_triggered()
{
    setStatusMsg(tr("GUI Language Changed"));
    qApp->removeTranslator(chineseTranslator);
}
