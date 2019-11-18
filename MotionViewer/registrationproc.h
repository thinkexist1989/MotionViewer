#ifndef REGISTRATIONPROC_H
#define REGISTRATIONPROC_H

#include <QObject>

class RegistrationProc : public QObject
{
    Q_OBJECT
public:
    explicit RegistrationProc(QObject *parent = nullptr);

signals:

public slots:
};

#endif // REGISTRATIONPROC_H