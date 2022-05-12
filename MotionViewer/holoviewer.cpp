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

/**
 * @brief HoloViewer::dataProc
 * @param data
 * 把收到的数据打印出来
 */
void HoloViewer::dataProc(const QString& data)
{
    qDebug() << "Received data is: " << data;
}

/**
 * @brief HoloViewer::changeEvent
 * @param event
 * 头部菜单栏--更换语言事件
 */
void HoloViewer::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}
