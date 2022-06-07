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

    ui->ldtAddress->setText("192.168.183.97");
    ui->ldtPort->setText("1366");
    ui->ldtLocalAddress->setText("192.168.183.97");
    ui->ldtLocalPort->setText("1366");

    //Default as Tcp Server.
    ui->cmbProtocol->setCurrentText("TCP");

}

bool HoloComm::write(QByteArray ba)
{
    switch(connectionType){
    case TCP_SERVER:
    case TCP_CLIENT:
        if(tcpSocket->write(ba) == ba.size()){
             return true;
        }

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

//execute按钮点击后会执行调用这里，但是两个形参是【0，无数据】
bool HoloComm::writeMatrix(int command, QVector<QMatrix4x4> matrixList)
{
//    测试用的 后期需要删掉
//    QMatrix4x4 mat(1,2,3,4,
//                   0,1,0,0,
//                   0,0,1,1,
//                   0,0,0,1);
//    matrixList.append(mat);

//    qDebug()<<sizeof(mat); //68

    QByteArray ba;
    int dataLen = matrixList.count()*16*4; //Every Matrix is 4x4

//  qDebug() <<"HoloComm::writeMatrix():准备发送数据了此时的command是"<<command;
//    qDebug() <<"发送:此时的matrixList的数据长度是"<<dataLen;

    ba.resize(dataLen+2);//设置字节数组的大小
    char* p = ba.data();
    //static_casr<type>（target）：将target的数据类型转化为type类型
    ba[0] = static_cast<char>(command);//封装报头--命令
    ba[1] = static_cast<char>(dataLen);//封装报头--数据长度



    //将数据添加到数据包的屁股后面
    for (int i = 0; i< matrixList.count(); i++) {//将matrixList中的数据拷贝到ba中，ba存的是要发生的数据信息
        //memcpy（target，resource）讲目标内存的数据拷贝到某个变量
        // at（i） 返回 i位置元素 所在vector的index
       memcpy(&(p[2+i*16*4]), matrixList.at(i).data(), 16*4); //column-major
    }

//    qDebug()<<ba;

    return write(ba);

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
    tcpServer = new QTcpServer();

    connect(tcpServer, &QTcpServer::newConnection,
            this, &HoloComm::tcpNewConnectionProc);

    if(!tcpServer->listen(QHostAddress(ui->ldtLocalAddress->text())
                          ,ui->ldtLocalPort->text().toUShort())) {
        qDebug() << tr("in holocomm-Tcp Server listen failed!");
        return  false;
    }
    else {
        qDebug() << tr("in holocomm-Tcp Server listen success!");
        return  true;
    }
}

//初始化TCP-Client
bool HoloComm::tcpClientInit()
{
    tcpSocket = new QTcpSocket();

    tcpSocket->connectToHost(QHostAddress(ui->ldtAddress->text()),ui->ldtPort->text().toUShort());

    if(tcpSocket->isValid()) {
        qDebug() << tr("Connected to Server: ") << tcpSocket->peerName();
        connect(tcpSocket, &QTcpSocket::readyRead, this, &HoloComm::tcpRecvDataProc);
        return true;
    }
    else
        return false;
}

bool HoloComm::udpInit()
{
    udpSocket = new QUdpSocket();

    udpSocket->bind(ui->ldtLocalPort->text().toUShort());

    connect(udpSocket, &QUdpSocket::readyRead, this, &HoloComm::udpRecvDataProc);

    return  true;
}

//接收Hololens发送的矩阵信息
void HoloComm::dataProc(QByteArray ba)
{
    if(ba.count() != 16*4)
        return;
//    qDebug()<<"HoloComm::dataProc：QT receive message"<<ba;
    //强制类型转化为float类型
    QMatrix4x4 mat(reinterpret_cast<float*>(ba.data()));
    //矩阵转置
    mat = mat.transposed();
    emit holoMatrixReady(mat);

    qDebug()<<"receive localtoworld"<<mat;
}

void HoloComm::tcpNewConnectionProc()
{
    tcpSocket = tcpServer->nextPendingConnection();
    qDebug() << tr("New connection from ").toLatin1() << tcpSocket->peerAddress();

    connect(tcpSocket, &QTcpSocket::readyRead,
            this, &HoloComm::tcpRecvDataProc);
    connect(tcpSocket, &QTcpSocket::disconnected,
            this, [=](){qDebug() << tr("Tcp Client is disconnected!");});

}

void HoloComm::tcpRecvDataProc()
{
    QByteArray buffer = tcpSocket->readAll();
    //write(buffer);
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

//hololens-ui-Connect按钮回调
void HoloComm::on_connectButton_clicked()
{
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
    //发送矩阵数据[命令],[矩阵数组]
    writeMatrix(command, matrixList);
}

void HoloComm::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}

void HoloComm::on_ldtAddress_cursorPositionChanged(int arg1, int arg2)
{

}
