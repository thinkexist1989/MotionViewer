#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdlg.h"
#include <QDockWidget>
#include <QDebug>
#include <QMessageBox>
#include <QMatrix4x4>
#include <QLineEdit>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) :
                                          QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          ndiComm(new NdiComm),
                                          ndiViewer(new NdiViewer),
                                          holoComm(new HoloComm),
                                          holoViewer(new HoloViewer),
                                          regiViewer(new RegiViewer),
                                          transform(new Transform),
                                          transformThread(new QThread)
{
    ui->setupUi(this);

    qRegisterMetaType<QMap<QString,QList<QVector3D>>>("QMap<QString,QList<QVector3D>>"); //register signal type
    qRegisterMetaType<QList<QMatrix4x4>>("QList<QMatrix4x4>");
    qRegisterMetaType<NdiTool>("NdiTool");
    qRegisterMetaType<QList<NdiTool>>("QList<NdiTool>");


    loadSettings();
    chineseTranslator = new QTranslator(this);
    chineseTranslator->load(":/translations/translation_zh.qm");

    ui->ndiCommDockWidget->setWidget(ndiComm);
    ui->ndiViewerDockWidget->setWidget(ndiViewer);
    ui->holoCommDockWidget->setWidget(holoComm);
    ui->holoViewerDockWidget->setWidget(holoViewer);
    ui->regiDockWidget->setWidget(regiViewer);

    this->tabifyDockWidget(ui->ndiCommDockWidget,ui->holoCommDockWidget);
    this->tabifyDockWidget(ui->holoCommDockWidget,ui->regiDockWidget);
    this->tabifyDockWidget(ui->ndiViewerDockWidget,ui->holoViewerDockWidget);

    ui->ndiCommDockWidget->raise();
    ui->ndiViewerDockWidget->raise();

    //connect(ndiComm, &NdiComm::initFinished, this, [=](QString msg){qDebug() << msg;});
    //connect(ndiComm, &NdiComm::dataReady, this, [=](QList<QVector3D> markers){ qDebug() << markers; });
    //connect(holoComm, &HoloComm::dataReady, this, [=](QString data){qDebug() << "Received data is: " << data;});

    transform->moveToThread(transformThread);
    transformThread->start();

    connect(ndiComm, &NdiComm::dataReady, ndiViewer, &NdiViewer::dataProc); //ndi msg come
    connect(holoComm, &HoloComm::dataReady,holoViewer, &HoloViewer::dataProc); //holo

    connect(ndiViewer, &NdiViewer::readyForTransform, transform, &Transform::transformProc); // transform
    connect(ndiViewer, &NdiViewer::toolsReady, transform, &Transform::toolsProc); // transform
    connect(transform, &Transform::readyForHololens, holoComm, &HoloComm::commandProc);   // send msg to HoloLens

    connect(holoComm, &HoloComm::holoMatrixReady, transform, &Transform::holoMatrixProc); // HoloLens return msg
    connect(regiViewer, &RegiViewer::poindCloudRegiMatReady, transform, &Transform::poindCloudRegiMatProc);

    //just for test
    //    QMatrix4x4 mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    //    qDebug() << "QMatrix is:" <<mat;

    //    qDebug() << "QMatrix data 2 is:" << mat.data()[2];
//    QPair<QString, int> a;
//    qDebug() << "a is:" << a.first << a.second;
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

//    QLineEdit* ldt = ui->holoCommDockWidget->findChild<QLineEdit*>("ldtAddress");
//    ldt->setText(settings.value("HoloLens IP", "192.168.0.1").toString());

//    ldt = ui->holoCommDockWidget->findChild<QLineEdit*>("ldtPort");
//    ldt->setText(settings.value("HoloLens IP", "192.168.0.1").toString());

//    QComboBox* cmbBox = ui->holoCommDockWidget->findChild<QComboBox*>("cmbType");
//    settings.setValue("Connection Type", cmbBox->currentText());
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat, this);
    settings.setValue("NdiViewer", ui->actionNdiViewer->isChecked());
    settings.setValue("HoloViewer", ui->actionHoloViewer->isChecked());

    QLineEdit* ldt = ui->holoCommDockWidget->findChild<QLineEdit*>("ldtAddress");
    settings.setValue("HoloLens IP", ldt->text());

    ldt = ui->holoCommDockWidget->findChild<QLineEdit*>("ldtPort");
    settings.setValue("HoloLens Port", ldt->text());

    QComboBox* cmbBox = ui->holoCommDockWidget->findChild<QComboBox*>("cmbType");
    settings.setValue("Connection Type", cmbBox->currentText());


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
