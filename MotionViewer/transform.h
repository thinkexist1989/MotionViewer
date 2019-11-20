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

    int currentCommand;

signals:
    void readyForHololens(int, QList<QMatrix4x4>);
public slots:
    void transformProc(int, QMap<QString,QList<QVector3D>>);
    void holoMatrixProc(QMatrix4x4);
    void poindCloudRegiMatProc(QMatrix4x4);

private:
    QMatrix4x4 poindCloudRegiMat;
    QMatrix4x4 holoMat;
    QMap<QString,QList<QVector3D>> tools;
    QList<QMatrix4x4> matrixList;
    void modelCalc();
    void calibrationNeedleCalc();
    void reviseMatrixCalc();
    void boneDrillCalc();

};

#endif // TRANSFORM_H
