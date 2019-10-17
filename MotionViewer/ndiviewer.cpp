#include "ndiviewer.h"
#include "ui_ndiviewer.h"
#include <QDebug>

NdiViewer::NdiViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiViewer)
{
    ui->setupUi(this);
}

NdiViewer::~NdiViewer()
{
    delete ui;
}

void NdiViewer::dataProc(QList<QVector3D> data)
{
    qDebug() << tr("Coordinate is received by NdiViewer, value is: ") << data;
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
