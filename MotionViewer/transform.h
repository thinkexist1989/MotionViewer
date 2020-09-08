#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QMatrix4x4>
#include "nditool.h"

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
    void needRegiMat();
public slots:
    void transformProc(int, QList<NdiTool>);
    void toolsProc(QList<NdiTool>);
    void holoMatrixProc(QMatrix4x4);
    void poindCloudRegiMatProc(QMatrix4x4);

private:
    QMatrix4x4 poindCloudRegiMat;
    QMatrix4x4 calibritionNeedleLastTimeMat;
    QMatrix4x4 holoMat;
    QMatrix4x4 HoloLensToHoloLensMarkerMatrix;
    QMatrix4x4 KinectMarkerToKinectMatrix;
    QMatrix4x4 modelResultLastTimeMat;
    QMatrix4x4 calibritionNeedleResultLastTimeMat;
    QList<NdiTool> existTools;
    QList<QMatrix4x4> matrixList;
    void modelCalc();
    void LoadCalibrationMatrix();
    void calibrationNeedleCalc();
    void reviseMatrixCalc();
    void boneDrillCalc();
    bool pCRead;
    QMatrix4x4 SetCoordination(QMap<int,QVector3D> markers);
    QMatrix4x4 SetCoordination1(QMap<int,QVector3D> markers);
    QMatrix4x4 SetCoordination2(QMap<int,QVector3D> markers);
    void getRegiMat();
};

#endif // TRANSFORM_H
