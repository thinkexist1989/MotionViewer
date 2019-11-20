#include "transform.h"

Transform::Transform(QObject *parent) :
    QObject(parent),
    currentCommand(0)
{

}

void Transform::transformProc(int command, QList<NdiTool> tools)
{
    //TODO: add transform code
    this->tools = tools;

    QList<QMatrix4x4> matrixList;
    switch(command) {
    case HOLO_MODEL:
        currentCommand = HOLO_MODEL;
        emit readyForHololens(HOLO_INFO, matrixList);
        break;
    case HOLO_CALI_NEEDLE:
        currentCommand = HOLO_CALI_NEEDLE;
        emit readyForHololens(HOLO_INFO, matrixList);
        break;
    case HOLO_REVISE_MATRIX:
        currentCommand = HOLO_REVISE_MATRIX;
        emit readyForHololens(HOLO_INFO, matrixList);
        break;
    case HOLO_BONE_DRILL:
        currentCommand = HOLO_BONE_DRILL;
        emit readyForHololens(HOLO_INFO, matrixList);
        break;
    default:
        break;
    }
}

void Transform::holoMatrixProc(QMatrix4x4 holoMat)
{
    qDebug() << "HoloLens send:" << holoMat;
    switch (currentCommand) {
    case HOLO_MODEL:
        modelCalc();
        emit readyForHololens(currentCommand, matrixList);
        currentCommand = HOLO_INFO;
        break;
    case HOLO_CALI_NEEDLE:
        calibrationNeedleCalc();
        emit readyForHololens(currentCommand, matrixList);
        currentCommand = HOLO_INFO;
        break;
    case HOLO_REVISE_MATRIX:
        reviseMatrixCalc();
        emit readyForHololens(currentCommand, matrixList);
        currentCommand = HOLO_INFO;
        break;
    case HOLO_BONE_DRILL:
        boneDrillCalc();
        emit readyForHololens(currentCommand, matrixList);
        break;
    default:
        break;
    }
}
QMatrix4x4 Transform::SetCoordination(QMap<int,QVector3D> markers)
{
    float m11=markers[1].x()-markers[0].x();
    float m12=markers[2].x()-markers[1].x();
    float m13=markers[1].y()*markers[2].z()-markers[1].z()*markers[2].y();
    float m14=markers[0].x();
    float m21=markers[1].y()-markers[0].y();
    float m22=markers[2].y()-markers[1].y();
    float m23=markers[1].z()*markers[2].x()-markers[1].x()*markers[2].z();
    float m24=markers[0].y();
    float m31=markers[1].z()-markers[0].z();
    float m32=markers[2].z()-markers[1].z();
    float m33=markers[1].x()*markers[2].y()-markers[1].y()*markers[2].x();
    float m34=markers[0].z();
    float m41=0;
    float m42=0;
    float m43=0;
    float m44=1;
    float a=sqrt(m33*m33+m32*m32+m31*m31);
    float b=sqrt(m23*m23+m22*m22+m21*m21);
    float c=sqrt(m13*m13+m12*m12+m11*m11);
    m13=m13/c;
    m12=m12/c;
    m11=m11/c;
    m33=m33/a;
    m32=m32/a;
    m31=m31/a;
    m23=m12*m31-m11*m32;
    m22=m11*m33-m13*m31;
    m21=m13*m32-m12*m33;
    m23=m23/b;
    m22=m22/b;
    m21=m21/b;
     QMatrix4x4 mat(m11,m12,m13,m14,
                    m21,m22,m23,m24,
                    m31,m32,m33,m34,
                    m41,m42,m43,m44);
    return mat;
}
void Transform::LoadCalibrationMatrix()
{ QMatrix4x4 HoloLensToHoloLensMarker(0.0975262771988006,-0.0431322212347106,0.994297861179990,0.0705467509747862,
                                      0.995227668296462,0.00748157594641098,-0.0972929302598390,0.0132814109105393,
                                      -0.00324245476923128,0.999041359260325,0.0436560302175016,-0.0752293725273258,
                                      0,0,0,1);
  QMatrix4x4 KinectMarkerToKinect(0.8169,-0.5766,-0.0114,-0.0521,
                                  -0.0090,0.0070,-0.9999,-0.0299,
                                  0.5766,0.8170, 0.0005,-0.0372,
                                  0,0, 0,1.0000);
  this->HoloLensToHoloLensMarkerMatrix=HoloLensToHoloLensMarker;
  this->KinectMarkerToKinectMatrix=KinectMarkerToKinect;
}
void Transform::poindCloudRegiMatProc(QMatrix4x4 mat)
{
    qDebug() << "Transform received RegiMat:" << mat;
    this->poindCloudRegiMat = mat;
}

void Transform::modelCalc()
{
    QMatrix4x4 result;
    QMatrix4x4 KinectToNDI;
    QMatrix4x4 HoloLensToNDI;
    QMatrix4x4 Registration;
    QMatrix4x4 ModelToHoloLens;
    QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
    QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
    QMatrix4x4 HoloLensToWorld;
    //get the markers' location of Kinect
    QMap<int,QVector3D> KinectMarkers;
    if(tools.contains(NdiTool("Kinect"))){
        int index = tools.indexOf(NdiTool("Kinect"));
       KinectMarkers=tools[index].getIndexAndCoordinate();
       KinectToNDI=SetCoordination(KinectMarkers);
    }
    //get the markers' location of HoloLens
    QMap<int,QVector3D> HoloLensMarkers;
    if(tools.contains(NdiTool("HoloLens"))){
        int index = tools.indexOf(NdiTool("HoloLens"));
      HoloLensMarkers=tools[index].getIndexAndCoordinate();
       HoloLensToNDI=SetCoordination(HoloLensMarkers);
    }
    //get the matrixs of pointcloud registration
    //缺一个判断，如果为空，弹出提示先读取配准结果
    Registration=poindCloudRegiMat;
    // get the HololensLocalToWorld matrix
    HoloLensToWorld=holoMat;
    //result

}

void Transform::calibrationNeedleCalc()
{
    QMatrix4x4 calibrationNeedleToNDI;
    QMatrix4x4 HoloLensToWorld;
    QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
    QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
    // get the HololensLocalToWorld matrix
    HoloLensToWorld=holoMat;
     //get the markers' location of Needle
    QMap<int,QVector3D> calibrationNeedleMarkers;
    if(tools.contains(NdiTool("calibrationNeedle"))){
        int index = tools.indexOf(NdiTool("calibrationNeedle"));
       calibrationNeedleMarkers=tools[index].getIndexAndCoordinate();
       calibrationNeedleToNDI=SetCoordination(calibrationNeedleMarkers);
    }
    this->calibritionNeedleLastTimeMat=calibrationNeedleToNDI;
}

void Transform::reviseMatrixCalc()
{
     QMap<int,QVector3D> calibrationNeedleMarkers;
     QMatrix4x4 calibrationNeedleToNDI;
     QMatrix4x4 LastTimeCalibrationNeedleToNDI;
     QMatrix4x4 reviseMatrix;
     QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
     QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
     QMatrix4x4 HoloLensToWorld;
     // get the HololensLocalToWorld matrix
     HoloLensToWorld=holoMat;
     //get the markers' location of Needle last time
     LastTimeCalibrationNeedleToNDI=calibritionNeedleLastTimeMat;
     //get the markers' location of Needle this time

     if(tools.contains(NdiTool("calibrationNeedle"))){
         int index = tools.indexOf(NdiTool("calibrationNeedle"));
        calibrationNeedleMarkers=tools[index].getIndexAndCoordinate();
        calibrationNeedleToNDI=SetCoordination(calibrationNeedleMarkers);
     }
}

void Transform::boneDrillCalc()
{
    QMatrix4x4 boneDrillToNDI;
    QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
    QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
    QMatrix4x4 HoloLensToWorld;
    // get the HololensLocalToWorld matrix
    HoloLensToWorld=holoMat;
    //get the markers' location of boneDrillCalc
    QMap<int,QVector3D> BoneDrillMarkers;
    if(tools.contains(NdiTool("BoneDrill"))){
        int index = tools.indexOf(NdiTool("BoneDrill"));
       BoneDrillMarkers=tools[index].getIndexAndCoordinate();
       boneDrillToNDI=SetCoordination(BoneDrillMarkers);
    }
}

