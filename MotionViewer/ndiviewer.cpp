#include "ndiviewer.h"
#include "ui_ndiviewer.h"

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
