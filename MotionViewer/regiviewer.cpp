#include "regiviewer.h"
#include "ui_regiviewer.h"
#include <QFileDialog>

RegiViewer::RegiViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegiViewer)
{
    ui->setupUi(this);
}

RegiViewer::~RegiViewer()
{
    delete ui;
}

void RegiViewer::showMatrix(QMatrix4x4 mat)
{
    ui->tblRegiMat->setRowCount(4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            ui->tblRegiMat->setItem(i,j, new QTableWidgetItem(QString::number(static_cast<double>(mat(i,j)))));
        }
    }
}

void RegiViewer::on_btnOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Poind Cloud Registration Matrix File"), QDir::currentPath(), "TXT (*.txt)");
    if(QFile::exists(fileName)){
        ui->ldtPointCloudRegiMatDir->setText(fileName);
    }
}

void RegiViewer::on_btnLoad_clicked()
{

}
