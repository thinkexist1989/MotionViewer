#include "holocomm.h"
#include "ui_holocomm.h"

HoloComm::HoloComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HoloComm)
{
    ui->setupUi(this);
    init();

}

HoloComm::~HoloComm()
{
    delete ui;
}

void HoloComm::init()
{
    QStringList protoList;
    protoList << "TCP" << "UDP";
    ui->cmbProtocol->addItems(protoList);

    QStringList typeList;
    typeList << tr("Server") << tr("Client");
    ui->cmbType->addItems(typeList);

    ui->ldtAddress->setText("192.168.0.105");
    ui->ldtPort->setText("12345");
    ui->ldtLocalAddress->setText("192.168.0.106");
    ui->ldtLocalPort->setText("12345");

    //Default as Tcp Server.
    ui->cmbProtocol->setCurrentText("TCP");

}

void HoloComm::on_cmbProtocol_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "TCP") {
        ui->cmbType->setEnabled(true);
    }
    else {
        ui->cmbType->setEnabled(false);
        ui->ldtPort->setEnabled(true);
        ui->ldtAddress->setEnabled(true);
        ui->ldtLocalPort->setEnabled(true);
        ui->ldtLocalAddress->setEnabled(false);
    }
}

void HoloComm::on_cmbType_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("Server")) {
        ui->ldtPort->setEnabled(false);
        ui->ldtAddress->setEnabled(false);
        ui->ldtLocalPort->setEnabled(true);
        ui->ldtLocalAddress->setEnabled(true);
    }
    else {
        ui->ldtPort->setEnabled(true);
        ui->ldtAddress->setEnabled(true);
        ui->ldtLocalPort->setEnabled(false);
        ui->ldtLocalAddress->setEnabled(false);
    }
}

bool HoloComm::tcpServerInit()
{
    return false;
}

bool HoloComm::tcpClientInit()
{
    return false;
}

bool HoloComm::udpInit()
{
    return  false;
}

void HoloComm::newConnectionProc()
{

}

void HoloComm::recvDataProc()
{

}

void HoloComm::disconnectProc()
{

}

void HoloComm::on_connectButton_clicked()
{
    if(ui->cmbProtocol->currentText() == "TCP") {
        if(ui->cmbType->currentText() == tr("Server")) { //tcp server
            if(tcpServerInit()) {
                ui->connectButton->setEnabled(false);
            }
        }
        else if(ui->cmbType->currentText() == tr("Client")){ // tcp client
            if(tcpClientInit()) {
                ui->connectButton->setEnabled(false);
            }
        }
    }
    else if(ui->cmbProtocol->currentText() == "UDP") {
        if(udpInit()) {
            ui->connectButton->setEnabled(false);
        }
    }
}

void HoloComm::on_disconnectButton_clicked()
{
    ui->connectButton->setEnabled(true);
}
