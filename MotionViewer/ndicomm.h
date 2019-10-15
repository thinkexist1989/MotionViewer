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

    friend class NdiCommProc; //Declare friend class NdiCommProc

    void initPort(); //refresh serial ports

private:
    Ui::NdiComm *ui;
    NdiCommProc *ndiCommProc;

    QSerialPort serialPort; //Serial port
    bool isPortOpened;
    bool isStarted;

    QSerialPort::Parity getParity(QString text);
    QSerialPort::DataBits getDataBits(QString text);
    QSerialPort::StopBits getStopBits(QString text);
    QSerialPort::FlowControl getFlowCtrl(QString text);

signals:
    void serialOpened(); //serial port open signal
    void serialClosed(); //serial port close signal

public slots:

private slots:
    void on_refreshButton_clicked();
    void on_openCloseButton_clicked();
    void on_startButton_clicked();
};


class NdiCommProc : public QObject
{
    Q_OBJECT
public:
    explicit NdiCommProc(QObject *parent = nullptr);
    ~NdiCommProc();

private:
    NdiComm *ndi;

signals:
    void dataReady();
};

#endif // NDICOMM_H
