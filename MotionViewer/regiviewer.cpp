#include "regiviewer.h"
#include "ui_regiviewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

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
    QString fileName = ui->ldtPointCloudRegiMatDir->text();
    if(!QFile::exists(fileName)){
        QMessageBox::warning(this,tr("Error File Path"), tr("Please input correct file path!"), QMessageBox::Ok);
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,tr("Open File Failed"), tr("Can not open file!"), QMessageBox::Ok);
        return;
    }


    QByteArray content = file.readAll();
    int index = content.indexOf("MATRIX");
    QString matString = content.mid(index);
    QTextStream in(&matString);
    in.readLine();
    QList<float> values;
    while(!in.atEnd()){
        QString row = in.readLine();
        float val;
        in >> val;
        values.push_back(val);
    }
    if(values.count() == 16)
        qDebug() << values;
}
