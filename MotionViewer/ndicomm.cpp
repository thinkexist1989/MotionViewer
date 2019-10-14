#include "ndicomm.h"
#include "ui_ndicomm.h"

NdiComm::NdiComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiComm)
{
    ui->setupUi(this);
}

NdiComm::~NdiComm()
{
    delete ui;
}
