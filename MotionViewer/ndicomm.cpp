#include "ndicomm.h"
#include "ui_ndicomm.h"
#include <QDebug>
#include <QtEndian>

NdiComm::NdiComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiComm),
    isPortOpened(false),
    isStarted(false)
{
    ui->setupUi(this);
    initPort();

    ndiCommProc = new NdiCommProc(this);
    qRegisterMetaType<QList<QVector3D>>("Coordinates");
    connect(ndiCommProc, &NdiCommProc::initFinished, this, [=](QString msg){emit this->initFinished(msg);});
    connect(ndiCommProc, &NdiCommProc::dataReady, this, [=](QList<QVector3D> data){
	this->markers = data;
        emit this->dataReady(markers);});

    ndiThread = new QThread(this);
}

NdiComm::~NdiComm()
{
    delete ui;

    ndiThread->terminate(); //Not recommand, but can work
    delete ndiThread;
    delete ndiCommProc;
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

void NdiComm::printThread()
{
    qDebug() << "Current Thread is: " << QThread::currentThreadId();
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
          //  printThread();
            ndiCommProc->moveToThread(ndiThread);
            ndiThread->start();
            ndiCommProc->initsensor();
            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, ndiCommProc, &NdiCommProc::get_data);
            timer->start(100);

            ui->startButton->setText(tr("Stop"));
            isStarted = true;
        }
        else {
            qDebug() << tr("Please Open Serial Port First!");
        }

    }
    else {
        timer->stop();
        ndiThread->terminate();

        ui->startButton->setText(tr("Start"));
        isStarted = false;
    }
}

void NdiComm::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}

/********************NdiCommProc*****************************/
/*
 * Processing time consuming operation
 */


NdiCommProc::NdiCommProc(NdiComm *ndi, QObject *parent) :
    QObject (parent),
    ndi (ndi)
{
    //ndi = dynamic_cast<NdiComm*>(parent);
	
}

NdiCommProc::~NdiCommProc()
{

}

void NdiCommProc::printThread()
{
    ndi->printThread();
    QList<QVector3D> markers;
    markers.push_back(QVector3D(1,2,3));
    markers.push_back(QVector3D(2,4,5));
    emit dataReady(markers);
}

void NdiCommProc::initsensor()
{
    //FOR SU SHUN
    msg = "1";
    ndi->write(msg);
    while (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);
    }
    QThread::msleep(5000);

    msg = "APIREV \r";
    ndi->write(msg);
    while (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);

    }
    msg = "VER:4A6EF\r";
    ndi->write(msg);
    while (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);

        //ui.textBrowser->insertPlainText(strDisplay);
    //ui.textBrowser->moveCursor(QTextCursor::End);
    //	return strDisplay;
    //	QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    }
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    QThread::msleep(500);
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    msg = "COMM 70001\r";
    ndi->write(msg);
    while (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);

        //ui.textBrowser->insertPlainText(strDisplay);
    //ui.textBrowser->moveCursor(QTextCursor::End);
    //	return strDisplay;
    //	QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    }
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    QThread::msleep(500);
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    msg = "COMM 50001\r";
    ndi->write(msg);
    while (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);

        //ui.textBrowser->insertPlainText(strDisplay);
    //ui.textBrowser->moveCursor(QTextCursor::End);
    //	return strDisplay;
    //	QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    }
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    QThread::msleep(500);
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    msg = "test\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);



    }
    QThread::msleep(5000);
    //QMessageBox::warning(NULL, "warning", "12", QMessageBox::Abort);
    msg = "VER:5662E\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


    }
    QThread::msleep(500);
    msg = "GETINFO:Config.*1110\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


    }
    QThread::msleep(500);
    msg = "GET:Device.*722D\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "INIT:E3A5\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GET:Device.*722D\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);

        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PHSR:0020FF\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PINIT:0131EA\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PHSR:0020FF\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Param.Tracking.*8D17\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Features.Firmware.Version0492\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Info.Status.Alerts340A\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Info.Status.New Alerts33A3\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);

        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Features.Hardware.Serial Number68E4\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "VER:4A6EF\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Features.Tools.*F635\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "SFLIST:03500F\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Param.Tracking.Selected VolumeC200\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PHINF:0100753CAF\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(5000);
    }
    QThread::msleep(500);
    msg = "GETINFO:SCU-0.Info.Status.New AlertsAF34\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);

        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:SCU-0.Info.Status.AlertsC917\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Info.Status.New Alerts33A3\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:Info.Status.Alerts340A\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:STB-0.Info.Status.New AlertsCC4F\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "GETINFO:STB-0.Info.Status.Alerts389B\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "TSTART:5423\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "BX:18033D6C\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "TSTOP:2C14\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "SET:Param.Tracking.Illuminator Rate=2237A\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PHRQ:*********1****A4C1\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:0200004E444900AF12000001000000000000010000000002DC32355A00000004000000040000000000403F000000000000000000000000000000000000000000000000610B\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:0200400000204100000000000000000000000000000000000000001F853D4285EBE74100000000000000003333B24200000000A4700DC2A4700D4200000000000000002B7A\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:020080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005DEA\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:0200C0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006EF1\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:02010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000803F00000000678F\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:020140000000000000803F00000000000000000000803F00000000000000000000803F00000000000000000000000000000000000000000000000000000000000000008535\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:020180000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009DD2\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:0201C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000AEC9\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:0202000000000000000000000000000000000000000000000000000000000000000000000000000000000000010203000000000000000000000000000000001F1F1F1FC0FA\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:020240090000004E44490000000000000000003837303034343900000000000000000000000000090101010100000000000000000000000000000000010101010000008755\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:020280000000000000000000000000008000290000000000000000000080BF0000000000000000000000000000000000000000000000000000000000000000000000002FB1\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PVWR:0202C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000AE82\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);

    msg = "PINIT:0230AA\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PHINF:0200753CEB\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "TSTART:5423\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "BX:18033D6C\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "TSTOP:2C14\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);
    msg = "PENA:02D9D3B\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        strDisplay = QString(requestData);


        QThread::msleep(500);
    }
    QThread::msleep(500);

    msg = "TSTART:5423\r";
    ndi->write(msg);
    if (this->ndi->waitForReadyRead(50))
    {
        requestData += this->ndi->readAll();
        strDisplay = QString(requestData);
        QThread::msleep(500);
    }
    QThread::msleep(500);

    //QString finish;

    //finish = " You can start collect data!";

}


void NdiCommProc::get_data()
{
    //FOR SU SHUN
    this->ndi->clear();
    msg = "BX:18033D6C\r";    //msg = "BX:1000FEAD\r";

    ndi->write(msg);
    int index = 0;

    requestData1.clear();

    while (this->ndi->waitForReadyRead(100))
    {

        QByteArray temp = this->ndi->readAll();
        requestData1 += temp;
    }

    const char* p = requestData1.data(); //get a pointer to received data

    index += 30;

    quint16 numofpoints = getNum<quint16>(&(p[index]));

    if (numofpoints > 8)
        index += 3;
    else
        index += 2;

    QList<QVector3D> markers;

    for (int i = 0; i < numofpoints; i++)
    {
        float coordinate[3];

        for (int j = 0; j < 3; j++) {
            coordinate[j] = getNum<float>(&(p[index]));
            index += 4;
        }

        markers.push_back(QVector3D(coordinate[0], coordinate[1], coordinate[2]));
    }
    emit dataReady(markers);

}

template<typename T> T NdiCommProc::getNum(const char *p)
{
    T temp;
    memcpy_s(&temp, sizeof (T), p, sizeof (T));
    return qFromBigEndian(temp); //From Big Endian to host byte order(x86 is Little Endian)
}


//void NdiCommProc::data_read()
//{
//    //FOR SU SHUN
//    this->ndi->clear();
//    msg = "BX:18033D6C\r";
//    //msg = "BX:1000FEAD\r";
//    //
//    ndi->write(msg);
//    int nSpot = 0;
//    //	int numofpoints = 0;
//    int p1 = 0;
//    int p2 = 0;
//    int p3 = 0;
//    int p4 = 0;
//    unsigned    char hexbyte[4];
//    requestData1.clear();
//    strDisplay.clear();
//    while (this->ndi->waitForReadyRead(100))
//    {

//        requestData = this->ndi->readAll();
//        requestData1 = requestData1 + requestData;
//    }
//    strDisplay = requestData1.toHex();
//    nSpot += 60;
//    int numofpoints = ConvertHexQString(strDisplay, 2, nSpot);
//    if (numofpoints > 8)
//        nSpot += 2;
//    nSpot += 4;
//    float data[1][3];
//    QList<QVector3D> markers;
//    for (int i = 0; i < numofpoints; i++)
//    {
//        p1 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p2 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p3 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p4 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        hexbyte[0] = p1;
//        hexbyte[1] = p2;
//        hexbyte[2] = p3;
//        hexbyte[3] = p4;
//        q = Hex_To_Decimal(hexbyte);
//        data[0][0] = q;
//        p1 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p2 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p3 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p4 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        hexbyte[0] = p1;
//        hexbyte[1] = p2;
//        hexbyte[2] = p3;
//        hexbyte[3] = p4;
//        q = Hex_To_Decimal(hexbyte);
//        data[0][1] = q;
//        p1 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p2 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p3 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        p4 = ConvertHexQString(strDisplay, 2, nSpot);
//        nSpot += 2;
//        hexbyte[0] = p1;
//        hexbyte[1] = p2;
//        hexbyte[2] = p3;
//        hexbyte[3] = p4;
//        q = Hex_To_Decimal(hexbyte);
//        data[0][2] = q;

//markers.push_back(QVector3D(data[0][0], data[0][1], data[0][2]));

//    }
//emit dataReady(markers);
//    requestData.clear();
	

//}

//int NdiCommProc::ConvertHexQString(QString ch, int i, int j)
//{
//    int u = 0;
//    for (int n = 0; n < i; n++)
//    {
//        if (ch[j + n] == '0')
//            u = u + (0 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '1')
//            u = u + (1 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '2')
//            u = u + (2 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '3')
//            u = u + (3 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '4')
//            u = u + (4 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '5')
//            u = u + (5 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '6')
//            u = u + (6 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '7')
//            u = u + (7 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '8')
//            u = u + (8 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == '9')
//            u = u + (9 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == 'a' || ch[j] == 'A')
//            u = u + (10 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == 'b' || ch[j] == 'B')
//            u = u + (11 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == 'c' || ch[j] == 'C')
//            u = u + (12 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == 'd' || ch[j] == 'D')
//            u = u + (13 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == 'e' || ch[j] == 'E')
//            u = u + (14 * (int)pow(16, i - n - 1));
//        if (ch[j + n] == 'f' || ch[j] == 'F')
//            u = u + (15 * (int)pow(16, i - n - 1));
//    }
//    return u;
//}
//float NdiCommProc::Hex_To_Decimal(unsigned char *Byte)//
//{

//    return *((float*)Byte);//
//}
