#include "holocomm.h"
#include "ui_holocomm.h"

HoloComm::HoloComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HoloComm)
{
    ui->setupUi(this);
}

HoloComm::~HoloComm()
{
    delete ui;
}
