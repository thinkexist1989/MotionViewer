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
//    delete tcpServer;
//    delete tcpSocket;
//    delete udpSocket;
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

bool HoloComm::write(QByteArray ba)
{
    switch(connectionType){
    case TCP_SERVER:
    case TCP_CLIENT:
        if(tcpSocket->write(ba) == ba.size())
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
        qDebug() << tr("Tcp Server listen failed!");
        return  false;
    }
    else {
        qDebug() << tr("Tcp Server listen success!");
        return  true;
    }
}

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

    return  true
            ;
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
}

void HoloComm::udpRecvDataProc()
{
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(),datagram.size());
        write(datagram);
        emit dataReady(datagram);
    }
}

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

void HoloComm::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}
