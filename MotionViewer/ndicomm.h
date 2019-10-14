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

private:
    Ui::NdiComm *ui;

signals:

public slots:
};

#endif // NDICOMM_H
