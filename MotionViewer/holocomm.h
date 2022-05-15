#ifndef HOLOCOMM_H
#define HOLOCOMM_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QByteArray>
#include <QMatrix4x4>
#include <QVector>
#include <transform.h>

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

    bool writeMatrix(int command, QVector<QMatrix4x4> matrixList);


private slots:
    void on_cmbProtocol_currentIndexChanged(const QString &arg1);

    void on_cmbType_currentIndexChanged(const QString &arg1);

private:
    Ui::HoloComm *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;

    /**
     * TCP_SERVER 0
     * TCP_CLIENT 1
     * UDP         2
     * NO_CONNECTION -1
     */
    int connectionType;

    bool tcpServerInit();
    bool tcpClientInit();
    bool udpInit();

    void dataProc(QByteArray ba); // process the data from HoloLens

signals:
    void dataReady(const QString&);
    void holoMatrixReady(const QMatrix4x4 &);

private slots:
    void tcpNewConnectionProc();
    void tcpRecvDataProc();

    void udpRecvDataProc();

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();

public slots:
    void commandProc(int, const QVector<QMatrix4x4> &);

protected:
    void changeEvent(QEvent *event);
};

#endif // HOLOCOMM_H
