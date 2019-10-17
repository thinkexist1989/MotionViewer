#include "holoviewer.h"
#include "ui_holoviewer.h"
#include <QDebug>

HoloViewer::HoloViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HoloViewer)
{
    ui->setupUi(this);
}

HoloViewer::~HoloViewer()
{
    delete ui;
}

void HoloViewer::dataProc(QString data)
{
    qDebug() << "Received data is: " << data;
}

void HoloViewer::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}
