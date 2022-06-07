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
    qRegisterMetaType<QVector<QVector3D>>("Coordinates");
    qRegisterMetaType<QVector<QVector3D>>("QVector<QVector3D>&");

    ui->setupUi(this);
    initPort();

    ndiCommProc = new NdiCommProc();

    connect(ndiCommProc, &NdiCommProc::initFinished, this, [=](QString msg){emit this->initFinished(msg);});
    connect(ndiCommProc, &NdiCommProc::dataReady, this, [=](const QVector<QVector3D>& data){
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

/**
 * @brief NdiComm::initPort
 * 端口初始化
 * 设置了NDI中的一些初始值。
 */
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
            //cmbPortName的值为：['com1','com7','com8']
            ui->cmbPortName->addItem(info.portName());
            tempSerial.close();
        }
    }

    //波特率
    QStringList baudList;
    baudList << "9600" << "14400" << "38400"
             << "57600" << "115200" << "921600" << "1228739";
    ui->cmbBuadrate->addItems(baudList);
    //设置默认值
    ui->cmbBuadrate->setCurrentText("1228739");


    //set parity combobox
    //校验位
    QStringList parityList;
    parityList << tr("None") << tr("Odd") << tr("Even");
    ui->cmbParity->addItems(parityList);
    //设置校验位 默认方式 None
    ui->cmbParity->setCurrentText("None");

    //set databits combobox
    //设置数据位
    QStringList dataBitsList;
    dataBitsList << "5" << "6" << "7" << "8";
    ui->cmbDataBits->addItems(dataBitsList);
    //数据位默认为8
    ui->cmbDataBits->setCurrentText("8");

    //set stopbits combobox
    //设置停止位
    QStringList stopBitsList;
    stopBitsList << "1" << "1.5" << "2";
    ui->cmbStopBits->addItems(stopBitsList);
    //停止位默认为1
    ui->cmbStopBits->setCurrentText("1");

    //设置流控制
    QStringList flowCtrlList;
    flowCtrlList << tr("None") << tr("Hardware") << tr("Software");
    ui->cmbFlowCtrl->addItems(flowCtrlList);
    //流控制默认为硬件控制
    ui->cmbFlowCtrl->setCurrentText(tr("Hardware"));
}

/**
 * @brief NdiComm::getParity
 * @param text
 * @return
 * 对NDI校验位做了一个封装
 */
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

/**
 * @brief NdiComm::getDataBits
 * @param text
 * @return
 * 对数据位做了一个封装
 */
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

/**
 * @brief NdiComm::getStopBits
 * @param text
 * @return
 * 对停止位做了一个封装
 */
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

/**
 * @brief NdiComm::printThread
 * @param front
 * 打印当前线程
 */
void NdiComm::printThread(QString front)
{
    qDebug()<< front << tr(" Thread is: ") << QThread::currentThreadId();
}

/**
 * @brief NdiComm::recvProc
 * 空方法
 */
void NdiComm::recvProc()
{

}

/**
 * @brief NdiComm::on_refreshButton_clicked
 * 点击刷新按钮时触发该事件
 * 再次初始化端口信息，或者是reset（重置）
 */
void NdiComm::on_refreshButton_clicked()
{
    initPort(); //refresh available ports
}

/**
 * @brief NdiComm::on_openCloseButton_clicked
 * 点击OPEN按钮时触发该事件
 */
void NdiComm::on_openCloseButton_clicked()
{
    if(!isPortOpened){ // serial not open
        serialName = ui->cmbPortName->currentText();
        baudRate = ui->cmbBuadrate->currentText().toInt();
        parity = getParity(ui->cmbParity->currentText());
        dataBits = getDataBits(ui->cmbDataBits->currentText());
        stopBits = getStopBits(ui->cmbStopBits->currentText());
        flowControl = getFlowCtrl(ui->cmbFlowCtrl->currentText());

        //发出串口打开的信号
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
                ui->openCloseButton->setText(tr("Open"));
                ui->openCloseButton->setIcon(QIcon(":/icon/res/start.ico"));
                qDebug() << "Close serial port success!";
            }
            else {
                qDebug() << "Close serial port failed!";
            }
        }
    }
}

/**
 * @brief NdiComm::on_startButton_clicked
 * 点击Start按钮触发该事件
 */
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

//翻译
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
    isRunning(false)
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
        QThread::msleep(10);
    }
}

void NdiCommProc::printThread(QString front)
{
    qDebug()<< front << tr(" Thread is: ") << QThread::currentThreadId();
    QVector<QVector3D> markers;
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

/**
 * @brief NdiCommProc::initsensor
 * @return
 * 初始化传感器
 */
bool NdiCommProc::initsensor()
{
    //FOR SU SHUN
    qDebug() << "Initializing~~";
    QVector<QByteArray> msgList;

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
//    QElapsedTimer timer;
//    timer.start();

    this->serialPort->clear();
    QByteArray msg = "BX:18033D6C\r";//"BX:18019AC\r";    //msg = "BX:1000FEAD\r";

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
            qDebug() <<"NdiCommProc: received data length is wrong!"<<recvbuf.size();
            return;
        }
    }

    QVector<QVector3D> markers;

    for (int i = 0; i < numofpoints; i++)
    {
        float coordinate[3] = {0};

        for (int j = 0; j < 3; j++) {
            coordinate[j] = getNum<float>(&(p[index]));
//            qDebug() << "Communication time consuming:" << coordinate[j];
            index += 4;
        }

        markers.push_back(QVector3D(coordinate[0], coordinate[1], coordinate[2]));
    }
    emit dataReady(markers);

//    QThread::msleep(10); //这个地方我怀疑是发的快了 所以出问题了
   // qDebug() << "Communication time consuming:" << (double)timer.nsecsElapsed()/(double)1000000 <<endl;
   // qDebug() << "Communication time consuming:" << timer.elapsed() << "ms";  //显示处理速度
}

template<typename T> T NdiCommProc::getNum(const char *p)
{
    T temp;
    memcpy(&temp, p, sizeof (T));
    return temp;
}

/*************************************************
Function:       calculate_crc16
Description:    通用的16位CRC校验算法
Input:          wCRCin：CRC16算法的初始值
                wCPoly：特征多项式
                wResultXOR：结果异或值
                input_invert：输入值是否反转
                ouput_invert：输出值是否反转
                puchMsg：开始校验的数据的起始地址
                usDataLen：校验的数据长度
Output:         无输出
Return:         16位CRC校验结果
Others:         example：CRC-16/CCITT由本函数实现则填充参数如下：
                calculate_crc(0,0x1021,0,true,true,puchMsg,usDataLen)
                NDI校验
                char* msg = "GET:Device.*";
                quint16 x = calculate_crc16(0,0x8005,0, true, true, msg,12);
*************************************************/
quint16 NdiCommProc::calculate_crc16(quint16 wCRCin,
                                       quint16 wCPoly,
                                       quint16 wResultXOR,
                                       bool input_invert,
                                       bool ouput_invert,
                                       const char *puchMsg,
                                       int usDataLen)
{
    quint8 wChar = 0;
    while (usDataLen--)
    {
        wChar = *(puchMsg++);
        if(input_invert)//输入值反转
        {
            quint8 temp_char = wChar;
            wChar=0;
            for(int i=0;i<8;++i)
            {
                if(temp_char&0x01)
                    wChar|=0x01<<(7-i);
                temp_char>>=1;
            }
        }
        wCRCin ^= (wChar << 8);
        for (int i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
                wCRCin = (wCRCin << 1) ^ wCPoly;
            else
                wCRCin = wCRCin << 1;
        }
    }
    if(ouput_invert)
    {
        quint16 temp_short = wCRCin;
        wCRCin=0;
        for(int i=0;i<16;++i)
        {
            if(temp_short&0x01)
                wCRCin|=0x01<<(15-i);
            temp_short>>=1;
        }
    }
    return (wCRCin^wResultXOR);
}
