#include "regiviewer.h"
#include "ui_regiviewer.h"

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
