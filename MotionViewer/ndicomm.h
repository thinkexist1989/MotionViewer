#ifndef NDICOMM_H
#define NDICOMM_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QVector3D>
#include <QVector>

extern QString                  serialName;
extern int                      baudRate;
extern QSerialPort::Parity      parity;
extern QSerialPort::StopBits    stopBits;
extern QSerialPort::DataBits    dataBits;
extern QSerialPort::FlowControl flowControl;
extern bool                     openSuccess;

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

    friend class NdiCommProc; //Declare friend class NdiCommProc

    void initPort(); //refresh serial ports

    QVector<QVector3D> markers; //storage for coordinates of markers

private:
    Ui::NdiComm *ui;
    NdiCommProc *ndiCommProc;
	NdiComm *ndiComm;
//    QThread *ndiThread;
//    QSerialPort serialPort; //Serial port
    
    //标识串口是否打开
    bool isPortOpened;
    bool isStarted;

    QSerialPort::Parity getParity(QString text);
    QSerialPort::DataBits getDataBits(QString text);
    QSerialPort::StopBits getStopBits(QString text);
    QSerialPort::FlowControl getFlowCtrl(QString text);

    void printThread(QString front); //Test for thread

public:
    //Use to read and write on serialPort.
//    inline void write(QByteArray ba) {if(isPortOpened) ndiCommProc->serialPort.write(ba);}
//    inline bool waitForReadyRead(int msecs) {return serialPort.waitForReadyRead(msecs);}
//    inline QByteArray readAll(){return serialPort.readAll();}
//    inline void clear() { serialPort.clear(); }

signals:
    void serialOpened(bool); //serial port open or close signal
    void commStarted(); //start with ndi signal

    void initFinished(QString);
    void dataReady(const QVector<QVector3D>& ); //发送检测到的markers

public slots:
    void recvProc();

private slots:
    void on_refreshButton_clicked();
    void on_openCloseButton_clicked();
    void on_startButton_clicked();

protected:
    void changeEvent(QEvent *event);
};

/**********************************/
/*
 * NdiComProc
 *
 *
 * ********************************/

class NdiCommProc : public QObject
{
    Q_OBJECT
public:
    explicit NdiCommProc(QObject *parent = nullptr);
    ~NdiCommProc();
    float q;
    //const char *msg;

    QByteArray requestData;
    QString strDisplay;

    QByteArray requestData1;

    QByteArray recvbuf;

    bool isRunning;

    QSerialPort* serialPort;
    QThread *ndiThread;

signals:
    void initFinished(QString);
    void dataReady(const QVector<QVector3D>& );

public slots:
    void openSerial(bool open);

    void ndiCommStart(); //start to communicate with Ndi

    void printThread(QString front);

    void get_data(); // Get data from NDI BY Yang Luo

//    void data_read(); //FOR SU SHUN

private:
    bool writeReadMsg(QByteArray sendmsg, QByteArray recvmsg = "", int delay_ms = 0, int read_ms = 10);
    bool initsensor();
    template<typename T> T getNum(const char* p);

    quint16 calculate_crc16(quint16 wCRCin,
                                           quint16 wCPoly,
                                           quint16 wResultXOR,
                                           bool input_invert,
                                           bool ouput_invert,
                                           const char *puchMsg,
                                           int usDataLen);

//    bool writeReadMsg(QByteArray msg);
//    bool datawrong=false;
//    int ConvertHexQString(QString ch, int i, int j);
//    float Hex_To_Decimal(unsigned char * Byte);

};

#endif // NDICOMM_H
