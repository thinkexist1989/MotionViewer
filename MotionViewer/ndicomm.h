#ifndef NDICOMM_H
#define NDICOMM_H

#include <QObject>

class NdiComm : public QObject
{
    Q_OBJECT
public:
    explicit NdiComm(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NDICOMM_H