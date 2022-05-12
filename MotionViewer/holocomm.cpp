#include "holocomm.h"
#include "ui_holocomm.h"

HoloComm::HoloComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HoloComm),
    connectionType(NO_CONNECTION)
{
    ui->setupUi(this);
    init();

}
//QT和HoloLens2的通信
HoloComm::~HoloComm()
{
    delete ui;
}

/**
 * hololen初始化
 * protoList定义了通信协议[TCP,UDP]
 * typeList定义了当前的通信角色[Server,Client]
 * @brief HoloComm::init
 */
void HoloComm::init()
{
    //定义了一个String类型的链表
    QStringList protoList;
    protoList << "TCP" << "UDP";
    //向holocomm.ui中的comProtocol下拉框添值[TCP,UDP]
    ui->cmbProtocol->addItems(protoList);

    QStringList typeList;
    //选择通信角色[Server,Client]
    typeList << tr("Server") << tr("Client");
    //向holocomm.ui中cmbType下拉款添加下值[Server,Client]
    ui->cmbType->addItems(typeList);


    //暂时没找到这些ui
    ui->ldtAddress->setText("192.168.0.105");
    ui->ldtPort->setText("12345");
    ui->ldtLocalAddress->setText("192.168.0.106");
    ui->ldtLocalPort->setText("12345");

    //Default as Tcp Server.默认使用TCP协议作为通讯协议--经过测试这里好像不会影响UI页面上的值
    ui->cmbProtocol->setCurrentText("TCP");

}

/**
 * @brief HoloComm::write
 * @param ba
 * @return
 * 宏定义：
 * TCP_SERVER     0
 * TCP_CLIENT     1
 * UDP            2
 * NO_CONNECTION -1
 */
bool HoloComm::write(QByteArray ba)
{
    switch(connectionType){
    case TCP_SERVER:
        //question:为什么这里是空呢？难道默认为选：TCP_CLIENT
    case TCP_CLIENT:
        if(tcpSocket->write(ba) == ba.size()) //question 不太理解
            return true;
        else
            return false;
    case UDP:
        if(udpSocket->writeDatagram(ba,QHostAddress(ui->ldtAddress->text()),ui->ldtPort->text().toUShort()) == ba.size())
            return true;
        else
            return false;
    default:
        return false;
    }
}

/**
 * @brief HoloComm::writeMatrix
 * @param command
 * @param matrixList
 * @return
 * 写入数据
 */
bool HoloComm::writeMatrix(int command, QVector<QMatrix4x4> matrixList)
{
    QByteArray ba;
    // 4阶方阵，一个4*4=16个元素，每个元素为4个字节。
    int dataLen = matrixList.count()*16*4; //Every Matrix is 4x4
    qDebug() << "dataLen is:" << dataLen;
    //设置QByteArray的数组大小 question：为啥+2
    ba.resize(dataLen+2);
    //返回一个指向字节数组ba的数据指针，并且指向第一个字符
    char* p = ba.data();

    ba[0] = static_cast<char>(command);
    ba[1] = static_cast<char>(dataLen);
    for (int i = 0; i< matrixList.count(); i++) {
        memcpy(&(p[2+i*16*4]), matrixList.at(i).data(), 16*4); //column-major
    }
    return write(ba);
}

/**
 * @brief HoloComm::on_cmbProtocol_currentIndexChanged
 * @param arg1
 * 当用户改变使用的协议时触发
 * 主要时根据用户选的协议类型来启用和禁用一些ui组件
 */
void HoloComm::on_cmbProtocol_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "TCP") {
        //当使用的是TCP协议时，启用
        ui->cmbType->setEnabled(true);
    }
    else {//当使用不是TCP协议时，启用和关闭一些ui组件，在此场景只设置了TPC,UDP个通信协议，因此以下是针对UDP来设置的
        ui->cmbType->setEnabled(false);
        ui->ldtPort->setEnabled(true);
        ui->ldtAddress->setEnabled(true);
        ui->ldtLocalPort->setEnabled(true);
        ui->ldtLocalAddress->setEnabled(false);
    }
}
/**
 * @brief HoloComm::on_cmbType_currentIndexChanged
 * @param arg1
 * 当用户改变通信角色时触发，
 */
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
/**
 * @brief HoloComm::tcpServerInit
 * @return
 * tcp服务器初始化
 */
bool HoloComm::tcpServerInit()
{

    tcpServer = new QTcpServer();

    connect(tcpServer, &QTcpServer::newConnection,
            this, &HoloComm::tcpNewConnectionProc);

    if(!tcpServer->listen(QHostAddress(ui->ldtLocalAddress->text())
                          ,ui->ldtLocalPort->text().toUShort())) {

        qDebug() << tr("Tcp Server listen failed!");
        return  false;
    }
    else {
        qDebug() << tr("Tcp Server listen success!");
        return  true;
    }
}
/**
 * @brief HoloComm::tcpClientInit
 * @return
 * tcp客户端初始化
 */
bool HoloComm::tcpClientInit()
{
    tcpSocket = new QTcpSocket();

    tcpSocket->connectToHost(QHostAddress(ui->ldtAddress->text()),ui->ldtPort->text().toUShort());

    /**
     * isValid（）：有可能存在BUG
     * socket.isValid()是判断socket当前是否可用。
     * 但是在调用了disconnectfromhost（）去断开连接，disconnectfromhost（）是会等待socket将未发送的数据发送完毕才会真正断开连接。
     * 而此时的socket是打开的[socket.isValid为true]
     * 优化方案：可以使用socket的state来优化。
     *
     */

    if(tcpSocket->isValid()) {
        //控制台输出连接到通信对象的name
        qDebug() << tr("Connected to Server: ") << tcpSocket->peerName();
        //读取数据
        connect(tcpSocket, &QTcpSocket::readyRead, this, &HoloComm::tcpRecvDataProc);
        return true;
    }
    else
        return false;
}

/**
 * @brief HoloComm::udpInit
 * @return
 * udp协议初始化
 */
bool HoloComm::udpInit()
{
    udpSocket = new QUdpSocket();

    udpSocket->bind(ui->ldtLocalPort->text().toUShort());

    connect(udpSocket, &QUdpSocket::readyRead, this, &HoloComm::udpRecvDataProc);

    return  true;
}

/**
 * @brief HoloComm::dataProc
 * @param ba 从socket中取出来的数据
 * 被tcpRecvDataProc（）调用、
 * 处理从socket中取出来的数据
 */
void HoloComm::dataProc(QByteArray ba)
{
    //question 16*4?? 矩阵是4*4的
    if(ba.count() != 16*4)
        return;

    //4阶方阵
    QMatrix4x4 mat(reinterpret_cast<float*>(ba.data()));
    mat = mat.transposed();
    //触发信号
    emit holoMatrixReady(mat);
    qDebug()<<"receive localtoworld"<<mat;
}

/**
 * @brief HoloComm::tcpNewConnectionProc
 * tcpServerInit()中调用了该方法去处理TCP连接
 */
void HoloComm::tcpNewConnectionProc()
{
    tcpSocket = tcpServer->nextPendingConnection();
    //toLatinl（）：将QString类型转化为QByteArray类型，使用的是ASCII编码
    qDebug() << tr("New connection from ").toLatin1() << tcpSocket->peerAddress();

    //读取数据
    connect(tcpSocket, &QTcpSocket::readyRead,
            this, &HoloComm::tcpRecvDataProc);

    //
    connect(tcpSocket, &QTcpSocket::disconnected,
            this, [=](){qDebug() << tr("Tcp Client is disconnected!");});

}

/**
 * @brief HoloComm::tcpRecvDataProc
 * 通过TCP读取数据
 */
void HoloComm::tcpRecvDataProc()
{
    //建立一个接受数据的缓冲区，将串口中的所有数据都拿出来。
    QByteArray buffer = tcpSocket->readAll();
    //write(buffer);
    //question：发送一个信号量？ 将接收到的数据传过去
    emit dataReady(buffer);

    dataProc(buffer);
}

void HoloComm::udpRecvDataProc()
{
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(),datagram.size());
        write(datagram);
        emit dataReady(datagram);        
        dataProc(datagram);
    }
}

/**
 * @brief HoloComm::on_connectButton_clicked
 * 在hololens.ui中点击connect按钮触发该事件
 */
void HoloComm::on_connectButton_clicked()
{
    //判断本次连接用的是哪种协议以及是服务器还是客户端。
    if(ui->cmbProtocol->currentText() == "TCP") {
        if(ui->cmbType->currentText() == tr("Server")) { //tcp server
            if(tcpServerInit()) {
                connectionType = TCP_SERVER;
                ui->connectButton->setEnabled(false);
            }
        }
        else if(ui->cmbType->currentText() == tr("Client")){ // tcp client
            if(tcpClientInit()) {
                connectionType = TCP_CLIENT;
                ui->connectButton->setEnabled(false);
            }
        }
    }
    else if(ui->cmbProtocol->currentText() == "UDP") {
        if(udpInit()) {
            connectionType = UDP;
            ui->connectButton->setEnabled(false);
        }
    }
}
/**
 * @brief HoloComm::on_disconnectButton_clicked
 * 在hololens.ui中点击disconnect按钮触发该事件--断开连接
 */
void HoloComm::on_disconnectButton_clicked()
{
    if(connectionType == TCP_SERVER) {
        tcpServer->close();
        tcpServer->disconnect();
        tcpSocket->close();
        tcpSocket->disconnect();
    }
    else if(connectionType == TCP_CLIENT) {
        tcpSocket->close();
    }
    else if(connectionType == UDP) {
        udpSocket->close();
    }

    connectionType = NO_CONNECTION;
    ui->connectButton->setEnabled(true);
}

void HoloComm::commandProc(int command, const QVector<QMatrix4x4>& matrixList)
{
    writeMatrix(command, matrixList);
}

/**
 * @brief HoloComm::changeEvent
 * @param event
 * 将UI页面中共的语言进行切换[中-英]
 */
void HoloComm::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}
