#include "ndicommthread.h"
#include <QElapsedTimer>

NdiCommThread::NdiCommThread(QObject *parent)
{
    _ndiComm = (NdiComm*)parent; //保存NdiComm指针
}

NdiCommThread::~NdiCommThread()
{
    _serialHandler->close();
    _mutex.lock();
    isQuit = true;
    _waitCondition.wakeOne();
    _mutex.unlock();
    wait();
}

bool NdiCommThread::connectNDI(const QString &name,
                                const int baud,
                                const QSerialPort::Parity par,
                                const QSerialPort::StopBits stop,
                                const QSerialPort::DataBits data,
                                const QSerialPort::FlowControl flow)
{
    _serialName = name;
    _baudRate = baud;
    _parity = par;
    _stopBits = stop;
    _dataBits = data;
    _flowControl = flow;

    const QMutexLocker locker(&_mutex);

    if(!isRunning())
        start(); //执行run()函数，开始接收数据
    else
        _waitCondition.wakeOne(); //TODO:暂时不太理解QWaitCondition

    return true;
}

bool NdiCommThread::disconnectNDI()
{
    _mutex.lock();
    isQuit = true;
    _waitCondition.wakeOne();
    _mutex.unlock();
    wait();

    _serialHandler->close();

    emit serialCloseSuccess(tr("%1 has been closed sucessfully").arg(_serialName));

    return true;
}

void NdiCommThread::run()
{
    if(_serialName.isEmpty()) //串口名为空
    {
        emit error(tr("No serial port Name is specified!"));
        return;
    }

    _serialHandler = QSharedPointer<QSerialPort>(new QSerialPort); //New一个QSerialport

    _serialHandler->setPortName(_serialName);

    if(!_serialHandler->open(QSerialPort::ReadWrite)) //打开串口失败
    {
        emit error(tr("Can not Open %1， error code %2!").arg(_serialName).arg(_serialHandler->error()));
        return;
    }

    _serialHandler->setBaudRate(_baudRate);
    _serialHandler->setParity(_parity);
    _serialHandler->setDataBits(_dataBits);
    _serialHandler->setStopBits(_stopBits);
    _serialHandler->setFlowControl(_flowControl);

    emit serialOpenSuccess(tr("%1 has been opened sucessfully").arg(_serialName));

    if(!initNDI())
        return;

    emit initNDISuccess(tr("Init NDI Successfully"));


    while(!isQuit)
    {
        get_data();
    }

}

bool NdiCommThread::writeReadMsg(QByteArray sendmsg, QByteArray recvmsg, int delay_ms, int read_ms)
{
    if(!isQuit){
        _serialHandler->write(sendmsg);
        QThread::msleep(static_cast<unsigned long>(delay_ms));
        QByteArray recvbuf;
        while (_serialHandler->waitForReadyRead(read_ms))
        {
            recvbuf.append(_serialHandler->readAll());
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

bool NdiCommThread::initNDI()
{
    qDebug() << "Initializing~~";
    QList<QByteArray> msgList;

    if(!writeReadMsg("VER 4\r", "Northern Digital Inc.",0,50)){
        qDebug() << tr("Waiting for reset...").toStdString().c_str();

        //reset to default config
        _serialHandler->setBaudRate(9600);
        _serialHandler->setParity(QSerialPort::NoParity);
        _serialHandler->setDataBits(QSerialPort::Data8);
        _serialHandler->setStopBits(QSerialPort::OneStop);
        _serialHandler->setFlowControl(QSerialPort::NoFlowControl);

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

    _serialHandler->setBaudRate(_baudRate);
    _serialHandler->setFlowControl(_flowControl);

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

void NdiCommThread::printThread(QString front)
{
    qDebug()<< front << tr(" Thread is: ") << QThread::currentThreadId();
}

void NdiCommThread::get_data()
{
    QElapsedTimer timer;
    timer.start();

    _serialHandler->clear();
    QByteArray msg = "BX:18033D6C\r";    //msg = "BX:1000FEAD\r";

    _serialHandler->write(msg);
    int index = 0;

    recvbuf.clear();

    while(_serialHandler->waitForReadyRead(10)){
        recvbuf.append(_serialHandler->readAll());
    }

    const char* p = recvbuf.data(); //get a pointer to received data

    index += 30;

    unsigned char numofpoints = getNum<unsigned char>(&(p[index]));

    if (numofpoints > 8)
        index += 1;

    index += 2;

    if(recvbuf.size() < (index + numofpoints*12)){
        if(_serialHandler->waitForReadyRead(50))
            return;
        recvbuf.append(_serialHandler->readAll());
        if(recvbuf.size() < (index + numofpoints*12)){
            qDebug() <<"NdiCommThread: received data length is wrong!";
            return;
        }
    }

    markers.clear(); //清除之前存储的markers

    for (int i = 0; i < numofpoints; i++)
    {
        float coordinate[3] = {0};

        for (int j = 0; j < 3; j++) {
            coordinate[j] = getNum<float>(&(p[index]));
            index += 4;
        }

        markers.push_back(QVector3D(coordinate[0], coordinate[1], coordinate[2]));
    }


    emit dataReady(markers); //发送信号
}

template<typename T> T NdiCommThread::getNum(const char *p)
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
quint16 NdiCommThread::calculate_crc16(quint16 wCRCin,quint16 wCPoly,quint16 wResultXOR,bool input_invert,bool ouput_invert,const char *puchMsg, int usDataLen)
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
