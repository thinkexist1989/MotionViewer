#include "holoviewer.h"
#include "ui_holoviewer.h"

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
