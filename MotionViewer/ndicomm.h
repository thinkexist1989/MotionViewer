#ifndef NDICOMM_H
#define NDICOMM_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QVector3D>
#include <QList>
#include "ndicommthread.h" //处理NDI串口通信的线程

namespace Ui {
    class NdiComm;
}

class NdiCommProc;

class NdiComm : public QWidget
{
    Q_OBJECT
public:
    explicit NdiComm(QWidget *parent = nullptr);
    ~NdiComm();

    friend class NdiCommThread; //Declare friend class NdiCommThread

    void initPort(); //refresh serial ports

    QVector<QVector3D> markers; //storage for coordinates of markers

private:
    Ui::NdiComm *ui;
    NdiCommProc *ndiCommProc;
	NdiComm *ndiComm;

    QSharedPointer<NdiCommThread> ndiThread;

    QString                  serialName;
    int                      baudRate;
    QSerialPort::Parity      parity;
    QSerialPort::StopBits    stopBits;
    QSerialPort::DataBits    dataBits;
    QSerialPort::FlowControl flowControl;
    bool                     openSuccess;
    
    bool isPortOpened;
    bool isStarted;

    QSerialPort::Parity getParity(QString text);
    QSerialPort::DataBits getDataBits(QString text);
    QSerialPort::StopBits getStopBits(QString text);
    QSerialPort::FlowControl getFlowCtrl(QString text);

//    void printThread(QString front); //Test for thread

public:
    //Use to read and write on serialPort.
//    inline void write(QByteArray ba) {if(isPortOpened) ndiCommProc->serialPort.write(ba);}
//    inline bool waitForReadyRead(int msecs) {return serialPort.waitForReadyRead(msecs);}
//    inline QByteArray readAll(){return serialPort.readAll();}
//    inline void clear() { serialPort.clear(); }

signals:
//    void serialOpened(bool); //serial port open or close signal
    void commStarted(); //start with ndi signal

//    void initFinished(QString);
    void dataReady(QVector<QVector3D>&);

public slots:
    void recvProc();

private slots:
    void on_refreshButton_clicked();
    void on_openCloseButton_clicked();
//    void on_startButton_clicked();

    void processError(const QString &s);
    void processTimout(const QString &s);

protected:
    void changeEvent(QEvent *event);
};

/**********************************/
/*
 * NdiComProc
 *
 *
 * ********************************/

//class NdiCommProc : public QObject
//{
//    Q_OBJECT
//public:
//    explicit NdiCommProc(QObject *parent = nullptr);
//    ~NdiCommProc();
//    float q;
//    //const char *msg;

//    QByteArray requestData;
//    QString strDisplay;

//    QByteArray requestData1;

//    QByteArray recvbuf;

//    bool isRunning;

//    QSerialPort* serialPort;
//    QThread *ndiThread;

//signals:
//    void initFinished(QString);
//    void dataReady(QList<QVector3D>);

//public slots:
//    void openSerial(bool open);

//    void ndiCommStart(); //start to communicate with Ndi

//    void printThread(QString front);

//    void get_data(); // Get data from NDI BY Yang Luo

////    void data_read(); //FOR SU SHUN

//private:
//    bool writeReadMsg(QByteArray sendmsg, QByteArray recvmsg = "", int delay_ms = 0, int read_ms = 10);
//    bool initsensor();
//    template<typename T> T getNum(const char* p);

////    bool writeReadMsg(QByteArray msg);
////    bool datawrong=false;
////    int ConvertHexQString(QString ch, int i, int j);
////    float Hex_To_Decimal(unsigned char * Byte);

//};

#endif // NDICOMM_H
