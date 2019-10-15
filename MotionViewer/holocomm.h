#ifndef HOLOCOMM_H
#define HOLOCOMM_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

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

private slots:
    void on_cmbProtocol_currentIndexChanged(const QString &arg1);

    void on_cmbType_currentIndexChanged(const QString &arg1);

private:
    Ui::HoloComm *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;

    bool tcpServerInit();
    bool tcpClientInit();
    bool udpInit();

private slots:
    void newConnectionProc();
    void recvDataProc();
    void disconnectProc();

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
};

#endif // HOLOCOMM_H
