#ifndef HOLOCOMM_H
#define HOLOCOMM_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QByteArray>

#define TCP_SERVER 0
#define TCP_CLIENT 1
#define UDP         2
#define NO_CONNECTION -1

namespace Ui {
class HoloComm;
}

class HoloComm : public QWidget
{
    Q_OBJECT

public:
    explicit HoloComm(QWidget *parent = nullptr);
    ~HoloComm();

    void init();
    bool write(QByteArray ba);


private slots:
    void on_cmbProtocol_currentIndexChanged(const QString &arg1);

    void on_cmbType_currentIndexChanged(const QString &arg1);

private:
    Ui::HoloComm *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;

    int connectionType; // 0->tcp server  1-> tcp client 2-> udp

    bool tcpServerInit();
    bool tcpClientInit();
    bool udpInit();

signals:
    void dataReady(QString);

private slots:
    void tcpNewConnectionProc();
    void tcpRecvDataProc();

    void udpRecvDataProc();

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();

protected:
    void changeEvent(QEvent *event);
};

#endif // HOLOCOMM_H
