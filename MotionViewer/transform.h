#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QObject>

class Transform : public QObject
{
    Q_OBJECT
public:
    explicit Transform(QObject *parent = nullptr);

signals:
    void readyForHololens(QList<QMatrix4x4>);
public slots:
    void registrationProc(QMap<QString,QList<QVector3D>>);
    void holoMatrixProc(QMatrix4x4);

};

#endif // TRANSFORM_H
