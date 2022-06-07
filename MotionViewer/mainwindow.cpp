/**
 * @file mainwindow.cpp
 * @author Yang Luo (luoyang@sia.cn)
 * @brief Source of class MainWindow
 * @version 0.1
 * @date 2020-09-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdlg.h"
#include <QDockWidget>
#include <QDebug>
#include <QMessageBox>
#include <QMatrix4x4>
#include <QLineEdit>
#include <QComboBox>
#include <QCloseEvent>

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent pointer to the parent QWidget, can be nullptr
 */
MainWindow::MainWindow(QWidget *parent) :
                                          QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          ndiComm(new NdiComm),
                                          ndiViewer(new NdiViewer),
                                          holoComm(new HoloComm),
                                          holoViewer(new HoloViewer),
                                          regiViewer(new RegiViewer),
                                          glViewer(new GLViewer),
                                          transform(new Transform),
                                          transformThread(new QThread)
{
    ui->setupUi(this);

    qRegisterMetaType<QMap<QString,QVector<QVector3D>>>("QMap<QString,QVector<QVector3D>>"); //!< register signal type
    qRegisterMetaType<QVector<QMatrix4x4>>("QVector<QMatrix4x4>");
    qRegisterMetaType<NdiTool>("NdiTool");
    qRegisterMetaType<QVector<NdiTool>>("QVector<NdiTool>");

    chineseTranslator = new QTranslator(this);
    chineseTranslator->load(":/translations/translation_zh.qm");

    ui->ndiCommDockWidget->setWidget(ndiComm);
//    ui->ndiViewerDockWidget->setWidget(ndiViewer);
    ui->holoCommDockWidget->setWidget(holoComm);
//    ui->holoViewerDockWidget->setWidget(holoViewer);
    ui->regiDockWidget->setWidget(regiViewer);
//    ui->glViewerDockWidget->setWidget(glViewer);

//    this->tabifyDockWidget(ui->ndiCommDockWidget,ui->holoCommDockWidget);
//    this->tabifyDockWidget(ui->holoCommDockWidget,ui->regiDockWidget);
//    this->tabifyDockWidget(ui->ndiViewerDockWidget,ui->holoViewerDockWidget);
//    this->tabifyDockWidget(ui->holoViewerDockWidget, ui->glViewerDockWidget);

//    ui->ndiCommDockWidget->raise();
//    ui->ndiViewerDockWidget->raise();

    ui->tabWidget->addTab(ndiViewer,"NDI Viewer");
    ui->tabWidget->addTab(holoViewer, "Holo Viewer");
    ui->tabWidget->addTab(glViewer, "GL Viewer");

    //connect(ndiComm, &NdiComm::initFinished, this, [=](QString msg){qDebug() << msg;});
    //connect(ndiComm, &NdiComm::dataReady, this, [=](QVector<QVector3D> markers){ qDebug() << markers; });
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

    connect(transform, &Transform::needRegiMat, regiViewer, &RegiViewer::needRegiMatProc);

    connect(ndiComm, &NdiComm::dataReady, glViewer, &GLViewer::dataProc);
    connect(ndiViewer, &NdiViewer::toolsReady, glViewer, &GLViewer::toolProc);
    connect(ndiViewer, &NdiViewer::toolsLoaded, glViewer, &GLViewer::toolsLoaded);
    //触发开始收集信号
    connect(transform,&Transform::collectNeedleTipData,ndiViewer, &NdiViewer::writeItDown);

    //just for test
    //    QMatrix4x4 mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    //    qDebug() << "QMatrix is:" <<mat;

    //    qDebug() << "QMatrix data 2 is:" << mat.data()[2];
//    QPair<QString, int> a;
//    qDebug() << "a is:" << a.first << a.second;

    loadSettings();
}

/**
 * @brief Destroy the Main Window:: Main Window object
 * 
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Show About Dialog
 * 
 */
void MainWindow::on_actionAbout_triggered()
{
    setStatusMsg(tr("Open About Dialog"));
    AboutDlg* aboutDlg = new AboutDlg(this);
    aboutDlg->show();
}

/**
 * @brief Show messages on status bar
 * 
 * @param msg The message to show
 */
void MainWindow::setStatusMsg(QString msg)
{
    ui->statusBar->showMessage(msg,2000);
}

/**
 * @brief Loading settings when the program started
 * 
 */
void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat, this);
    ui->actionNdiViewer->setChecked(settings.value("NdiViewer",true).toBool());
    ui->actionHoloViewer->setChecked(settings.value("HoloViewer",true).toBool());

    QLineEdit* ldt = ui->holoCommDockWidget->findChild<QLineEdit*>("ldtAddress");
    ldt->setText(settings.value("HoloLens IP", "192.168.0.1").toString());

    ldt = ui->holoCommDockWidget->findChild<QLineEdit*>("ldtPort");
    ldt->setText(settings.value("HoloLens Port", "12345").toString());

    ldt = ui->regiDockWidget->findChild<QLineEdit*>("ldtPointCloudRegiMatDir");
    ldt->setText(settings.value("RegiMat Dir", "../").toString());

    QComboBox* cmbBox = ui->holoCommDockWidget->findChild<QComboBox*>("cmbType");
    cmbBox->setCurrentText(settings.value("Connection Type", "Server").toString());
}

/**
 * @brief Saving settings when the program closed
 * 
 */
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

    ldt = ui->regiDockWidget->findChild<QLineEdit*>("ldtPointCloudRegiMatDir");
    settings.setValue("RegiMat Dir", ldt->text());

}

/**
 * @brief Translating the UI to different language
 * 
 * @param event Process QEvent::LanguageChange
 */
void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QMainWindow::changeEvent(event);
    }
}

/**
 * @brief Close Event
 * 
 * @param event 
 */
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

/**
 * @brief Process actionNdiViwer toggled
 * 
 * @param arg1 Whether the actionNdiViewer is checked (the action name should be actionNdiComm properly)
 */
void MainWindow::on_actionNdiViewer_toggled(bool arg1)
{
    if(!arg1) {
        ui->ndiCommDockWidget->close();
//        ui->ndiViewerDockWidget->close();
    }
    else {
        ui->ndiCommDockWidget->show();
//        ui->ndiViewerDockWidget->show();
    }
}

/**
 * @brief Process action HoloViewer toggled
 * 
 * @param arg1 Whether the actionHoloViewer is checked (the action name should be actionHoloComm properly)
 */
void MainWindow::on_actionHoloViewer_toggled(bool arg1)
{
    if(!arg1) {
        ui->holoCommDockWidget->close();
//        ui->holoViewerDockWidget->close();
    }
    else {
        ui->holoCommDockWidget->show();
//        ui->holoViewerDockWidget->show();
    }
}

/**
 * @brief Change UI language to Chinese
 * 
 */
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

void MainWindow::on_ndiCommDockWidget_visibilityChanged(bool visible)
{
//    if(!visible && ui->actionNdiViewer->isChecked())
        ui->actionNdiViewer->setChecked(visible);
}

void MainWindow::on_holoCommDockWidget_visibilityChanged(bool visible)
{
    ui->actionHoloViewer->setChecked(visible);
}

void MainWindow::on_actionRegi_toggled(bool arg1)
{
    if(!arg1) {
        ui->regiDockWidget->close();
    }
    else {
        ui->regiDockWidget->show();
    }
}

void MainWindow::on_regiDockWidget_visibilityChanged(bool visible)
{
    ui->actionRegi->setChecked(visible);
}

void MainWindow::on_actionDisplayMode_triggered()
{
    static int mode = 0;

    if(mode)
    {
        mode = 0;
        glViewer->setDrawMode(mode);
        ui->actionDisplayMode->setIcon(QIcon(":icon/res/fill.png"));
    }
    else
    {
        mode = 1;
        glViewer->setDrawMode(mode);
        ui->actionDisplayMode->setIcon(QIcon(":icon/res/line.png"));
    }
}

void MainWindow::on_actionNdi_triggered()
{
    ui->tabWidget->addTab(ndiViewer,"NDI Viewer");
}

void MainWindow::on_actionFrontView_triggered()
{
    glViewer->camera->frontView();
}

void MainWindow::on_actionRightView_triggered()
{
    glViewer->camera->rightView();
}

void MainWindow::on_actionTopView_triggered()
{
    glViewer->camera->topView();
}

void MainWindow::on_actionAxo_triggered()
{
    glViewer->camera->AxoView();
}

void MainWindow::on_actionProjection_triggered()
{
    static int projMode = 0;

    if(projMode)
    {
        projMode = 0;
        glViewer->setProjectionMode(projMode);
        ui->actionProjection->setIcon(QIcon(":icon/res/persp.png"));
    }
    else
    {
        projMode = 1;
        glViewer->setProjectionMode(projMode);
        ui->actionProjection->setIcon(QIcon(":/icon/res/ortho.png"));
    }
}
