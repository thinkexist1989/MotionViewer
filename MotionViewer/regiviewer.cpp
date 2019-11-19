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

void RegiViewer::on_btnOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Poind Cloud Registration Matrix File"), QDir::currentPath(), "TXT (*.txt)");
    if(QFile::exists(fileName)){
        ui->ldtPointCloudRegiMatDir->setText(fileName);
    }
}
