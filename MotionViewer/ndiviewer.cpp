#include "ndiviewer.h"
#include "ui_ndiviewer.h"
#include <QDebug>

NdiViewer::NdiViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiViewer)
{
    ui->setupUi(this);
    init();
    QList<QVector3D> markers;
    markers.push_back(QVector3D(1.1,2.1,3.1));
    markers.push_back(QVector3D(2.2,4.4,5.9));
    markers.push_back(QVector3D(5.6,7.8,5.4));

    refreshMarkersView(markers);

    refreshMatrixView(virtualTransformMatrix);

}

NdiViewer::~NdiViewer()
{
    delete ui;
}

void NdiViewer::init()
{
    ui->tblMarkersView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblMatrixView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblMatrixView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //set comboBox Functions
    QStringList functionLists;
    functionLists << tr("1. Virtual Tool Pose") << tr("2. Actual Tool Pose") << tr("3. Update Calibration Matrix");
    ui->cmbSteps->addItems(functionLists);
}

void NdiViewer::refreshMarkersView(QList<QVector3D> data)
{

//    ui->tblMarkersView->horizontalHeader()->setStretchLastSection(true);

    int count = data.count();

    ui->tblMarkersView->setRowCount(count);

    for(int i = 0; i< count; i++){
        QVector3D marker = data.at(i);
        ui->tblMarkersView->setItem(i,0,new QTableWidgetItem(QString::number(static_cast<double>(marker.x()))));
        ui->tblMarkersView->setItem(i,1,new QTableWidgetItem(QString::number(static_cast<double>(marker.y()))));
        ui->tblMarkersView->setItem(i,2,new QTableWidgetItem(QString::number(static_cast<double>(marker.z()))));
    }
}

void NdiViewer::refreshMatrixView(QMatrix4x4 mat)
{
    ui->tblMatrixView->setRowCount(4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            ui->tblMatrixView->setItem(i,j, new QTableWidgetItem(QString::number(static_cast<double>(mat(i,j)))));
        }
    }
}

void NdiViewer::dataProc(QList<QVector3D> data)
{
    qDebug() << tr("Coordinate is received by NdiViewer, value is: ") << data;
    refreshMarkersView(data);
}

void NdiViewer::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}

void NdiViewer::on_btnExec_clicked()
{
    const int index = ui->cmbSteps->currentIndex();
    switch (index) {
    case 0://step 1
        virtualTransformMatrix = getVirtualTransformMatrix();
        refreshMatrixView(virtualTransformMatrix);
        break;
    case 1://step 2
        realTransformMatrix = getRealTransformMatrix();
        refreshMatrixView(realTransformMatrix);
        break;
    case 2://step 3
        calibrationMatrix = getCalibrationMatrix();
        refreshMatrixView(calibrationMatrix);
        break;
    default:
        break;
    }
}

void NdiViewer::on_cmbSteps_currentIndexChanged(int index)
{
    switch (index) {
    case 0://step 1
        refreshMatrixView(virtualTransformMatrix);
        break;
    case 1://step 2
        refreshMatrixView(realTransformMatrix);
        break;
    case 2://step 3
        refreshMatrixView(calibrationMatrix);
        break;
    default:
        break;
    }
}


QMatrix4x4 NdiViewer::getVirtualTransformMatrix()
{
    QMatrix4x4 mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}

QMatrix4x4 NdiViewer::getRealTransformMatrix()
{
    QMatrix4x4 mat(1.1,2.2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}

QMatrix4x4 NdiViewer::getCalibrationMatrix()
{
    QMatrix4x4 mat(1.5,2.7,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}
