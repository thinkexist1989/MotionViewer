#ifndef NDICOMMTHREAD_H
#define NDICOMMTHREAD_H

#include <QThread>          //多线程
#include <QSerialPort>      // 处理串口通信
#include <QSharedPointer>
#include <QMutex>           //线程锁
#include <QWaitCondition>

#include <QVector> //性能优于<QList>
#include <QVector3D> //存储NDI点

#include <QDebug> //

class NdiComm;

class NdiCommThread : public QThread
{
    Q_OBJECT
public:
    explicit NdiCommThread(QObject *parent = nullptr); //构造，传入NdiComm指针
    ~NdiCommThread(); //析构

    bool connectNDI(const QString& name = "COM1",
                     const int baud = 1228739,
                     const QSerialPort::Parity par = QSerialPort::NoParity,
                     const QSerialPort::StopBits stop = QSerialPort::OneStop,
                     const QSerialPort::DataBits data = QSerialPort::Data8,
                     const QSerialPort::FlowControl flow = QSerialPort::HardwareControl); //

    bool disconnectNDI();

    quint16 calculate_crc16(quint16 wCRCin,
                            quint16 wCPoly,
                            quint16 wResultXOR,
                            bool input_invert,
                            bool ouput_invert,
                            const char *puchMsg,
                            int usDataLen); //crc16校验

    void printThread(QString front); //Test for thread

public slots:
    void get_data(); // Get data from NDI BY Yang Luo

public:
    bool isQuit = false; //是否退出标志位

    QByteArray requestData;
    QString strDisplay;

    QByteArray requestData1;

    QByteArray recvbuf;

    QVector<QVector3D> markers; //存储检测到的markers

signals:
    void error(const QString& s); // 发生错误signal
    void timeout(const QString& s); // 发送或接收超时 signal

//    void initFinished(QString& s);
    void serialOpenSuccess(const QString& s);
    void serialCloseSuccess(const QString& s);

    void initNDISuccess(const QString& s);

    void dataReady(QVector<QVector3D>& markers);

private:
    void run() override;

    bool writeReadMsg(QByteArray sendmsg, QByteArray recvmsg = "", int delay_ms = 0, int read_ms = 10);
    bool initNDI();
    template<typename T> T getNum(const char* p);

private:
    NdiComm* _ndiComm; //存储NdiComm指针

    QSharedPointer<QSerialPort> _serialHandler; //串口指针

private:
    QString                  _serialName;    //串口名
    int                      _baudRate;      //波特率
    QSerialPort::Parity      _parity;        //校验位
    QSerialPort::StopBits    _stopBits;      //停止位
    QSerialPort::DataBits    _dataBits;      //数据位
    QSerialPort::FlowControl _flowControl;   //流控制

    QMutex  _mutex;                          //线程锁
    QWaitCondition _waitCondition;


};

#endif // NDICOMMTHREAD_H
