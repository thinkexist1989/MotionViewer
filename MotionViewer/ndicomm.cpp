#include "ndicomm.h"
#include "ui_ndicomm.h"
#include <QDebug>
#include <QElapsedTimer>

QString serialName = "COM1";
int baudRate = 1228739;
QSerialPort::Parity parity = QSerialPort::NoParity;
QSerialPort::StopBits stopBits = QSerialPort::OneStop;
QSerialPort::DataBits dataBits = QSerialPort::Data8;
QSerialPort::FlowControl flowControl = QSerialPort::HardwareControl;

bool openSuccess = false;


NdiComm::NdiComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiComm),
    isPortOpened(false),
    isStarted(false)
{
    ui->setupUi(this);
    initPort();

    ndiCommProc = new NdiCommProc();
    qRegisterMetaType<QList<QVector3D>>("Coordinates");
    connect(ndiCommProc, &NdiCommProc::initFinished, this, [=](QString msg){emit this->initFinished(msg);});
    connect(ndiCommProc, &NdiCommProc::dataReady, this, [=](QList<QVector3D> data){
	this->markers = data;
        emit this->dataReady(markers);});

    connect(this, &NdiComm::serialOpened, ndiCommProc, &NdiCommProc::openSerial, Qt::BlockingQueuedConnection);
    connect(this, &NdiComm::commStarted, ndiCommProc, &NdiCommProc::ndiCommStart);
}

NdiComm::~NdiComm()
{
    delete ui;

    ndiCommProc->isRunning = false;
    //ndiThread->quit(); //Not recommand, but can work
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
    baudList << "9600" << "14400" << "38400"
             << "57600" << "115200" << "921600" << "1228739";
    ui->cmbBuadrate->addItems(baudList);
    ui->cmbBuadrate->setCurrentText("1228739");

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
    ui->cmbFlowCtrl->setCurrentText(tr("Hardware"));
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

void NdiComm::printThread(QString front)
{
    qDebug()<< front << tr(" Thread is: ") << QThread::currentThreadId();
}

void NdiComm::recvProc()
{

}

void NdiComm::on_refreshButton_clicked()
{
    initPort(); //refresh available ports
}

void NdiComm::on_openCloseButton_clicked()
{
    if(!isPortOpened){ // serial not open
        serialName = ui->cmbPortName->currentText();
        baudRate = ui->cmbBuadrate->currentText().toInt();
        parity = getParity(ui->cmbParity->currentText());
        dataBits = getDataBits(ui->cmbDataBits->currentText());
        stopBits = getStopBits(ui->cmbStopBits->currentText());
        flowControl = getFlowCtrl(ui->cmbFlowCtrl->currentText());

        emit serialOpened(true);

        if(openSuccess){ // serial open success
            isPortOpened = true;
            ui->openCloseButton->setText(tr("Close"));
            ui->openCloseButton->setIcon(QIcon(":/icon/res/stop.ico"));
            qDebug() << "Open serial port success!";
        }
        else { // serial open failed
            qDebug() << "Open serial port failed!";
        }
    }
    else { // serial is already open
        if(isStarted){ // is communicating with ndi
            qDebug() << tr("Please stop running first!");
        }
        else { // not communicating with ndi
            emit serialOpened(false);

            if(!openSuccess){ // serial close success
                isPortOpened = false;
                ui->openCloseButton->setText(tr("Close"));
                ui->openCloseButton->setIcon(QIcon(":/icon/res/start.ico"));
                qDebug() << "Close serial port success!";
            }
            else {
                qDebug() << "Close serial port failed!";
            }
        }
    }
}

void NdiComm::on_startButton_clicked()
{
    if(!isStarted){ //Not start
        if(isPortOpened){ // serial is already opened
            emit commStarted();
            ui->startButton->setText(tr("Stop"));
            isStarted = true;
            ndiCommProc->isRunning = true;
        }
        else { // serial not open
            qDebug() << tr("Please Open Serial Port First!");
        }

    }
    else { // is already start
        ndiCommProc->isRunning = false;
        ui->startButton->setText(tr("Start"));
        isStarted = false;

//        ndiThread->quit();
//        connect(ndiCommProc->ndiThread, &QThread::finished, this, [=](){
//            ui->startButton->setText(tr("Start"));
//            isStarted = false;});

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
 *
 *
 *  Processing time consuming operation
 *
 *
 ************************************************************/


NdiCommProc::NdiCommProc(QObject *parent) :
    QObject (parent),
    isRunning(true)
{
    ndiThread = new QThread();


    this->moveToThread(ndiThread);

    serialPort = new QSerialPort();

    serialPort->moveToThread(ndiThread);

    //connect(ndiThread, &QThread::started, this, &NdiCommProc::ndiCommStart);
    //connect(ndiThread, &QThread::finished, ndiThread, &QThread::deleteLater);

    ndiThread->start();
	
}

NdiCommProc::~NdiCommProc()
{

}

void NdiCommProc::openSerial(bool open)
{
    if(open){
        serialPort->setPortName(serialName);
        if(serialPort->open(QIODevice::ReadWrite)){ //open serial port success
            serialPort->setBaudRate(9600);
            serialPort->setParity(QSerialPort::NoParity);
            serialPort->setDataBits(QSerialPort::Data8);
            serialPort->setStopBits(QSerialPort::OneStop);
            serialPort->setFlowControl(QSerialPort::NoFlowControl);
            openSuccess = true;
        }
        else {
            openSuccess = false;
        }
    }
    else {
        if(isRunning == false){
            serialPort->close();
            openSuccess = false;
        }
        else {
            qDebug() << tr("Please Open Serial Port First!");
        }

    }
}

void NdiCommProc::ndiCommStart()
{
    if(!initsensor()){ //init Ndi

        return;
    }
    while (isRunning) {
//        data_read();
        get_data();
    }
}

void NdiCommProc::printThread(QString front)
{
    qDebug()<< front << tr(" Thread is: ") << QThread::currentThreadId();
    QList<QVector3D> markers;
    markers.push_back(QVector3D(1,2,3));
    markers.push_back(QVector3D(2,4,5));
    emit dataReady(markers);
}

//bool NdiCommProc::writeReadMsg(QByteArray msg)
//{
//    if(isRunning){
//        serialPort->write(msg);
//        while (this->serialPort->waitForReadyRead(50))
//        {
//            this->serialPort->readAll();
//        }
//        return  true;
//    }
//    else {
//        return false;
//    }
//}

bool NdiCommProc::writeReadMsg(QByteArray sendmsg, QByteArray recvmsg, int delay_ms, int read_ms)
{
    if(isRunning){
        serialPort->write(sendmsg);
        QThread::msleep(static_cast<unsigned long>(delay_ms));
        QByteArray recvbuf;
        while (this->serialPort->waitForReadyRead(read_ms))
        {
            recvbuf.append(this->serialPort->readAll());
        }
        if(recvbuf.contains(recvmsg)){
            return  true;
        }
        else{
            qDebug() << "Response error!";
            return false;
        }
    }
    else {
        return false;
    }
}

bool NdiCommProc::initsensor()
{
    //FOR SU SHUN
    qDebug() << "Initializing~~";
    QList<QByteArray> msgList;

    if(!writeReadMsg("VER 4\r", "Northern Digital Inc.",0,50)){
        qDebug() << "Waiting for reset...";
        writeReadMsg("RESET 0", "RESET", 8000, 50);
    }

    //QThread::msleep(5000);

    msgList << "APIREV \r" << "COMM 70001\r";

    foreach(const QByteArray msg, msgList){
        if(!writeReadMsg(msg,"",0,50))
            return false;
        QThread::msleep(50);
    }

    QThread::msleep(60);

    serialPort->setBaudRate(baudRate);
    serialPort->setFlowControl(flowControl);

    msgList.clear();
    msgList << "VER 5\r" << "GETINFO:Config.*1110\r" << "GET:Device.*722D\r";

    foreach(const QByteArray msg, msgList){
        if(!writeReadMsg(msg, "" , 0, 50))
            return false;
        QThread::msleep(50);
    }

    msgList.clear();
    msgList << "INIT:E3A5\r" << "GET:Device.*722D\r" << "PHSR:0020FF\r" << "PINIT:0131EA\r"
            << "PHSR:0020FF\r" << "GETINFO:Param.Tracking.*8D17\r" << "GETINFO:Features.Firmware.Version0492\r"
            << "GETINFO:Info.Status.Alerts340A\r" << "GETINFO:Info.Status.New Alerts33A3\r" << "GETINFO:Features.Hardware.Serial Number68E4\r"
            << "VER 4\r" << "GETINFO:Features.Tools.*F635\r" << "SFLIST:03500F\r" << "GETINFO:Param.Tracking.Selected VolumeC200\r"
            << "PHINF:0100753CAF\r" << "GETINFO:SCU-0.Info.Status.New AlertsAF34\r" << "GETINFO:SCU-0.Info.Status.AlertsC917\r"
            << "GETINFO:Info.Status.New Alerts33A3\r" << "GETINFO:Info.Status.Alerts340A\r" << "GETINFO:STB-0.Info.Status.New AlertsCC4F\r"
            << "GETINFO:STB-0.Info.Status.Alerts389B\r" << "TSTART:5423\r" << "BX:18033D6C\r" << "TSTOP:2C14\r" << "SET:Param.Tracking.Illuminator Rate=2237A\r"
            << "PHRQ:*********1****A4C1\r"
            << "PVWR:0200004E444900AF12000001000000000000010000000002DC32355A00000004000000040000000000403F000000000000000000000000000000000000000000000000610B\r"
            << "PVWR:0200400000204100000000000000000000000000000000000000001F853D4285EBE74100000000000000003333B24200000000A4700DC2A4700D4200000000000000002B7A\r"
            << "PVWR:020080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005DEA\r"
            << "PVWR:0200C0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006EF1\r"
            << "PVWR:02010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000803F00000000678F\r"
            << "PVWR:020140000000000000803F00000000000000000000803F00000000000000000000803F00000000000000000000000000000000000000000000000000000000000000008535\r"
            << "PVWR:020180000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009DD2\r"
            << "PVWR:0201C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000AEC9\r"
            << "PVWR:0202000000000000000000000000000000000000000000000000000000000000000000000000000000000000010203000000000000000000000000000000001F1F1F1FC0FA\r"
            << "PVWR:020240090000004E44490000000000000000003837303034343900000000000000000000000000090101010100000000000000000000000000000000010101010000008755\r"
            << "PVWR:020280000000000000000000000000008000290000000000000000000080BF0000000000000000000000000000000000000000000000000000000000000000000000002FB1\r"
            << "PVWR:0202C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000AE82\r"
            << "PINIT:0230AA\r"
            << "PHINF:0200753CEB\r"
            << "TSTART:5423\r"
            << "BX:18033D6C\r"
            << "TSTOP:2C14\r"
            << "PENA:02D9D3B\r"
            << "TSTART:5423\r"
            << "BX:18033D6C\r";

    foreach(const QByteArray msg, msgList){
        if(!writeReadMsg(msg, "", 0, 50))
            return false;
        QThread::msleep(50);
    }

    qDebug() << "Init OK!";
    return true;
}


void NdiCommProc::get_data()
{
    QElapsedTimer timer;
    timer.start();

    this->serialPort->clear();
    QByteArray msg = "BX:18033D6C\r";    //msg = "BX:1000FEAD\r";

    serialPort->write(msg);
    int index = 0;

    recvbuf.clear();

    while(this->serialPort->waitForReadyRead(10)){
        recvbuf.append(this->serialPort->readAll());
    }

    const char* p = recvbuf.data(); //get a pointer to received data

    index += 30;

    unsigned char numofpoints = getNum<unsigned char>(&(p[index]));

    if (numofpoints > 8)
        index += 1;

    index += 2;

    if(recvbuf.size() < (index + numofpoints*12)){
        if(this->serialPort->waitForReadyRead(50))
            return;
        recvbuf.append(this->serialPort->readAll());
        if(recvbuf.size() < (index + numofpoints*12)){
            qDebug() <<"NdiCommProc: received data length is wrong!";
            return;
        }
    }

    QList<QVector3D> markers;

    for (int i = 0; i < numofpoints; i++)
    {
        float coordinate[3] = {0};

        for (int j = 0; j < 3; j++) {
            coordinate[j] = getNum<float>(&(p[index]));
            index += 4;
        }

        markers.push_back(QVector3D(coordinate[0], coordinate[1], coordinate[2]));
    }
    emit dataReady(markers);
    //QThread::msleep(10);这个地方我怀疑是发的快了 所以出问题了
   // qDebug() << "Communication time consuming:" << (double)timer.nsecsElapsed()/(double)1000000 <<endl;
   // qDebug() << "Communication time consuming:" << timer.elapsed() << "ms";  //显示处理速度
}

template<typename T> T NdiCommProc::getNum(const char *p)
{
    T temp;
    memcpy(&temp, p, sizeof (T));
    return temp;
}

//void NdiCommProc::data_read()
//{
//    //FOR SU SHUN
//    this->serialPort->clear();
//    QByteArray msg = "BX:18033D6C\r";
//    //msg = "BX:1000FEAD\r";
//    //
//    serialPort->write(msg);
//    int nSpot = 0;
//    //	int numofpoints = 0;
//    int p1 = 0;
//    int p2 = 0;
//    int p3 = 0;
//    int p4 = 0;
//    unsigned    char hexbyte[4];
//    requestData1.clear();
//    strDisplay.clear();
//    while (this->serialPort->waitForReadyRead(100))
//    {
//        requestData = this->serialPort->readAll();
//        requestData1 = requestData1 + requestData;
//    }
//    strDisplay = requestData1.toHex();
//    nSpot += 60;
//    int numofpoints = ConvertHexQString(strDisplay, 2, nSpot);
//    if (numofpoints > 8)
//        nSpot += 2;
//    nSpot += 4;
//    if(requestData1.length() != nSpot + numofpoints*12)
//    {
//        qDebug()<<"data length wrong,continue get data!";
//        requestData = this->serialPort->readAll();
//        requestData1 = requestData1 + requestData;
//        strDisplay = requestData1.toHex();
//    }
//    if(requestData1.length() == nSpot + numofpoints*12)
//    {

//        qDebug()<<"data length right!";
//    }
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

//    emit dataReady(markers);
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
