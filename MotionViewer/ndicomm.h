#pragma warning(disable:4995)
#pragma warning(disable:4819)

#ifndef NDICOMM_H
#define NDICOMM_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
    class NdiComm;
}

class NdiComm : public QWidget
{
    Q_OBJECT
public:
    explicit NdiComm(QWidget *parent = nullptr);
    ~NdiComm();

    void initPort(); //初始化串口，当点击刷新时也会调用，重新扫描当前可用串口

private:
    Ui::NdiComm *ui;
    QSerialPort serialPort; //串口
    bool isPortOpened; //串口是否打开

    QSerialPort::Parity getParity(QString text);
    QSerialPort::DataBits getDataBits(QString text);
    QSerialPort::StopBits getStopBits(QString text);
    QSerialPort::FlowControl getFlowCtrl(QString text);

signals:
    void serialOpened(); //串口打开信号
    void serialClosed(); //串口关闭信号

public slots:
private slots:
    void on_refreshButton_clicked();
    void on_openCloseButton_clicked();
};

#endif // NDICOMM_H
