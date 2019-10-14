#pragma warning(disable:4995)
#pragma warning(disable:4819)

#include "ndicomm.h"
#include "ui_ndicomm.h"
#include <QDebug>

NdiComm::NdiComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiComm),
    isPortOpened(false)
{
    ui->setupUi(this);
    initPort();
}

NdiComm::~NdiComm()
{
    delete ui;
}

void NdiComm::initPort()
{
    //查询当前可用串口，并将其加入选择框之中
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << tr("Name: ") << info.portName();
        qDebug() << tr("Description: ") << info.description();
        qDebug() << tr("Manufacturer: ") << info.manufacturer();

        QSerialPort tempSerial;
        tempSerial.setPort(info);
        if(tempSerial.open(QIODevice::ReadWrite)){
            ui->cmbPortName->addItem(info.portName());
            tempSerial.close();
        }
    }

    //设置波特率选择框
    QStringList baudList;
    baudList << "4800" << "9600" << "19200"
             << "38400" << "57600" << "115200";
    ui->cmbBuadrate->addItems(baudList);
    ui->cmbBuadrate->setCurrentText("9600");

    //设置奇偶校验位
    QStringList parityList;
    parityList << tr("None") << tr("Odd") << tr("Even");
    ui->cmbParity->addItems(parityList);
    ui->cmbParity->setCurrentText("None");

    //设置数据位
    QStringList dataBitsList;
    dataBitsList << "5" << "6" << "7" << "8";
    ui->cmbDataBits->addItems(dataBitsList);
    ui->cmbDataBits->setCurrentText("8");

    //设置停止位
    QStringList stopBitsList;
    stopBitsList << "1" << "1.5" << "2";
    ui->cmbStopBits->addItems(stopBitsList);
    ui->cmbStopBits->setCurrentText("1");

    QStringList flowCtrlList;
    flowCtrlList << tr("None") << tr("Hardware") << tr("Software");
    ui->cmbFlowCtrl->addItems(flowCtrlList);
    ui->cmbFlowCtrl->setCurrentText(tr("None"));
}

QSerialPort::Parity NdiComm::getParity(QString text)
{
    if(text == tr("None"))
        return QSerialPort::NoParity;
    else if(text == tr("Odd"))
        return QSerialPort::OddParity;
    else if(text == tr("Even"))
        return QSerialPort::EvenParity;
    else
        return QSerialPort::UnknownParity;
}

QSerialPort::DataBits NdiComm::getDataBits(QString text)
{
    switch(text.toInt()){
    case 5:
        return QSerialPort::Data5;
    case 6:
        return QSerialPort::Data6;
    case 7:
        return QSerialPort::Data7;
    case 8:
        return QSerialPort::Data8;
    default:
        return QSerialPort::UnknownDataBits;
    }
}

QSerialPort::StopBits NdiComm::getStopBits(QString text)
{
    if(text == "1")
        return QSerialPort::OneStop;
    else if(text == "1.5")
        return  QSerialPort::OneAndHalfStop;
    else if(text == "2")
        return  QSerialPort::TwoStop;
    else
        return QSerialPort::UnknownStopBits;
}

QSerialPort::FlowControl NdiComm::getFlowCtrl(QString text)
{
    if(text == tr("None"))
        return  QSerialPort::NoFlowControl;
    else if(text == tr("Hardware"))
        return QSerialPort::HardwareControl;
    else if(text == tr("Software"))
        return  QSerialPort::SoftwareControl;
    else
        return QSerialPort::UnknownFlowControl;
}

void NdiComm::on_refreshButton_clicked()
{
    initPort(); //重新查看可用串口
}

void NdiComm::on_openCloseButton_clicked()
{
    if(!isPortOpened){
        serialPort.setPortName(ui->cmbPortName->currentText());
        if(serialPort.open(QIODevice::ReadWrite)){ //串口打开成功
            serialPort.setBaudRate(ui->cmbBuadrate->currentText().toInt());
            serialPort.setParity(getParity(ui->cmbParity->currentText()));
            serialPort.setDataBits(getDataBits(ui->cmbDataBits->currentText()));
            serialPort.setStopBits(getStopBits(ui->cmbStopBits->currentText()));
            serialPort.setFlowControl(getFlowCtrl(ui->cmbFlowCtrl->currentText()));

            isPortOpened = true;
            emit serialOpened();
        }
        else {
            qDebug() << tr("Failed to open serial port");
        }

    }
    else {
        serialPort.close();

        emit serialClosed(); //弹出串口关闭信号

        isPortOpened = false;
    }
}
