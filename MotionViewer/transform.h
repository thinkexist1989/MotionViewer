#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QMatrix4x4>

#define HOLO_INFO          0
#define HOLO_MODEL         1
#define HOLO_CALI_NEEDLE   2
#define HOLO_REVISE_MATRIX 3
#define HOLO_BONE_DRILL    4

class Transform : public QObject
{
    Q_OBJECT
public:
    explicit Transform(QObject *parent = nullptr);

signals:
    void readyForHololens(int, QList<QMatrix4x4>);
public slots:
    void transformProc(QMap<QString,QList<QVector3D>>);
    void holoMatrixProc(QMatrix4x4);

};

#endif // TRANSFORM_H
