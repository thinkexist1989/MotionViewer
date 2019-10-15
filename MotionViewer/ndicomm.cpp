#include "ndicomm.h"
#include "ui_ndicomm.h"
#include <QDebug>

NdiComm::NdiComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiComm),
    isPortOpened(false),
    isStarted(false)
{
    ui->setupUi(this);
    initPort();

    ndiCommProc = new NdiCommProc(this);
}

NdiComm::~NdiComm()
{
    delete ui;
}

void NdiComm::initPort()
{
    //query available ports
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

    //set buadrate combobox
    QStringList baudList;
    baudList << "4800" << "9600" << "19200"
             << "38400" << "57600" << "115200";
    ui->cmbBuadrate->addItems(baudList);
    ui->cmbBuadrate->setCurrentText("9600");

    //set parity combobox
    QStringList parityList;
    parityList << tr("None") << tr("Odd") << tr("Even");
    ui->cmbParity->addItems(parityList);
    ui->cmbParity->setCurrentText("None");

    //set databits combobox
    QStringList dataBitsList;
    dataBitsList << "5" << "6" << "7" << "8";
    ui->cmbDataBits->addItems(dataBitsList);
    ui->cmbDataBits->setCurrentText("8");

    //set stopbits combobox
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
        return QSerialPort::NoParity;
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
        return QSerialPort::Data8;
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
        return QSerialPort::OneStop;
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
        return QSerialPort::NoFlowControl;
}

void NdiComm::on_refreshButton_clicked()
{
    initPort(); //refresh available ports
}

void NdiComm::on_openCloseButton_clicked()
{
    if(!isPortOpened){
        serialPort.setPortName(ui->cmbPortName->currentText());
        if(serialPort.open(QIODevice::ReadWrite)){ //open serial port success
            serialPort.setBaudRate(ui->cmbBuadrate->currentText().toInt());
            serialPort.setParity(getParity(ui->cmbParity->currentText()));
            serialPort.setDataBits(getDataBits(ui->cmbDataBits->currentText()));
            serialPort.setStopBits(getStopBits(ui->cmbStopBits->currentText()));
            serialPort.setFlowControl(getFlowCtrl(ui->cmbFlowCtrl->currentText()));

            isPortOpened = true;
            ui->openCloseButton->setText(tr("Close"));
            ui->openCloseButton->setIcon(QIcon(":/icon/res/stop.ico"));
            emit serialOpened();
        }
        else {
            qDebug() << tr("Failed to open serial port");
        }

    }
    else {
        serialPort.close();

        emit serialClosed(); //emit serialClosed signal;

        isPortOpened = false;
        ui->openCloseButton->setText(tr("Close"));
        ui->openCloseButton->setIcon(QIcon(":/icon/res/start.ico"));
    }
}

void NdiComm::on_startButton_clicked()
{
    if(!isStarted){ //Not start
        if(isPortOpened){

        }
    }
}


/********************NdiCommProc*****************************/
/*
 * Processing time consuming operation
 */

NdiCommProc::NdiCommProc(QObject *parent) :
    QObject (parent)
{
    ndi = dynamic_cast<NdiComm*>(parent);
}

NdiCommProc::~NdiCommProc()
{

}
