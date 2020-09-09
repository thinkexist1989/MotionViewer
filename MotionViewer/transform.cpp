#include "transform.h"
#include <QFileDialog>

Transform::Transform(QObject *parent) :
    QObject(parent),
    currentCommand(0),
    pCRead(false)
{
}

void Transform::transformProc(int command, const QVector<NdiTool>& existTools)//这个existTools有啥用？
{
    //TODO: add transform code
    //this->tools = tools;
    if(!pCRead)
        getRegiMat();

    QVector<QMatrix4x4> matrixList;
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

void Transform::toolsProc(const QVector<NdiTool>& Tools)
{
    this->existTools = Tools;
}


QMatrix4x4 Transform::SetCoordination(QMap<int, QVector3D> &markers)
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
     QMatrix4x4 mat;
     mat.setRow(0,QVector4D(m11,m12,m13,m14/1000));
     mat.setRow(1,QVector4D(m21,m22,m23,m24/1000));
     mat.setRow(2,QVector4D(m31,m32,m33,m34/1000));
     mat.setRow(3,QVector4D(m41,m42,m43,m44));
     //qDebug()<<"ToolcoordinationMatrix"<<mat;
     return mat;
 }
QMatrix4x4 Transform::SetCoordination1(QMap<int, QVector3D> &markers)
{
     QMatrix4x4 mat;
     QVector3D A=markers[0];
    // qDebug()<<"vector A"<<A;
     QVector3D B=markers[1];
    // qDebug()<<"vector B"<<B;
     QVector3D C=markers[2];
    // qDebug()<<"vector C"<<C;
     QVector3D AA=(B+C)/2-A;
     //qDebug()<<"vector AA"<<AA;
     QVector3D BB=B-A;
     //qDebug()<<"vector BB"<<BB;
     QVector3D CC=QVector3D::crossProduct(AA,BB);
     CC=CC.normalized();
     //qDebug()<<"CC"<<CC;
     AA=AA.normalized();
     //BB=QVector3D::normal(C,A);
     BB=QVector3D::crossProduct(CC,AA);
     BB=BB.normalized();
     //QGenericMatrix(QMatrix4x4).

    mat.setColumn(0, QVector4D(AA,0));
    mat.setColumn(1, QVector4D(BB,0));
    mat.setColumn(2, QVector4D(CC,0));
    mat.setColumn(3, QVector4D(markers[0]/1000,1));
    //qDebug()<<"Toolcoordination1Matrix"<<mat;
    return mat;

}
QMatrix4x4 Transform::SetCoordination2(QMap<int, QVector3D> &markers)
{
     QMatrix4x4 mat;
     QVector3D A=markers[0];
    // qDebug()<<"vector A"<<A;
     QVector3D B=markers[1];
    // qDebug()<<"vector B"<<B;
     QVector3D C=markers[2];
    // qDebug()<<"vector C"<<C;
     QVector3D AA=A-B;
     //qDebug()<<"vector AA"<<AA;
     QVector3D BB=C-A;
     //qDebug()<<"vector BB"<<BB;
     QVector3D CC=QVector3D::crossProduct(AA,BB);
     CC=CC.normalized();
     //qDebug()<<"CC"<<CC;
     AA=AA.normalized();
     //BB=QVector3D::normal(C,A);
     BB=QVector3D::crossProduct(CC,AA);
     BB=BB.normalized();
     //QGenericMatrix(QMatrix4x4).

    mat.setColumn(0, QVector4D(AA,0));
    mat.setColumn(1, QVector4D(BB,0));
    mat.setColumn(2, QVector4D(CC,0));
    mat.setColumn(3, QVector4D(markers[0]/1000,1));
    qDebug()<<"Toolcoordination1Matrix"<<mat;
    return mat;

}
void Transform::getRegiMat()
{
    emit needRegiMat();
}


void Transform::LoadCalibrationMatrix()
{ QMatrix4x4 HoloLensToHoloLensMarker;
    HoloLensToHoloLensMarker.setRow(0,QVector4D(0.0975262771988006,-0.0431322212347106,0.994297861179990,0.0705467509747862));
    HoloLensToHoloLensMarker.setRow(1,QVector4D(0.995227668296462,0.00748157594641098,-0.0972929302598390,0.0132814109105393));
    HoloLensToHoloLensMarker.setRow(2,QVector4D(-0.00324245476923128,0.999041359260325,0.0436560302175016,-0.0752293725273258));
    HoloLensToHoloLensMarker.setRow(3,QVector4D(0,0,0,1));
    //qDebug()<<"HoloLensToHoloLensMarker"<<HoloLensToHoloLensMarker;
  QMatrix4x4 KinectMarkerToKinect;
//  KinectMarkerToKinect.setRow(0,QVector4D(0.8169,-0.5766,-0.0114,-0.0521));
//  KinectMarkerToKinect.setRow(1,QVector4D(-0.0090,0.0070,-0.9999,-0.0299));
//  KinectMarkerToKinect.setRow(2,QVector4D(0.5766,0.8170, 0.0005,-0.0372));
//  KinectMarkerToKinect.setRow(3,QVector4D(0,0,0,1));
//  KinectMarkerToKinect.setRow(0,QVector4D(0.805047667579374	,0.0242791253042955	,-0.592713064643818	,0.0414454673757110));
//  KinectMarkerToKinect.setRow(1,QVector4D(-0.593098936442153,0.0135986281423361,-0.805014738314669,-0.0215208694997023));
//  KinectMarkerToKinect.setRow(2,QVector4D(-0.0114849691421504,0.999612725702863,0.0253474672639905,-0.0367470357651656));
//  KinectMarkerToKinect.setRow(3,QVector4D(0,0,0,1));
    KinectMarkerToKinect.setRow(0,QVector4D(0.805047667579375	,-0.593098936442152	,-0.0114849691421508	,-0.0465516202259655));
    KinectMarkerToKinect.setRow(1,QVector4D(0.0242791253042953	,0.0135986281423364	,0.999612725702861	,0.0360191991886339));
    KinectMarkerToKinect.setRow(2,QVector4D(-0.592713064643818	,-0.805014738314668	,0.0253474672639903	,0.00817209714135227));
    KinectMarkerToKinect.setRow(3,QVector4D(0,0,0,1));
  //qDebug()<<"KinectMarkerToKinect"<<KinectMarkerToKinect;
 QMatrix4x4 HoloLensCameratoHoloLens;
 HoloLensCameratoHoloLens.setRow(0,QVector4D(0.999773739057218,	0.0210033940735367,	0.00561886502047147,-0.00991192830221874));
 HoloLensCameratoHoloLens.setRow(1,QVector4D(0.0213648747610139,-0.997427839881842,-0.0684774762115394,0.00326366088660478));
 HoloLensCameratoHoloLens.setRow(2,QVector4D(0.00421721391800663,0.0684935369659415,-0.997624078482386,0.0625722174495811));
 HoloLensCameratoHoloLens.setRow(3,QVector4D(0,0,0,1));
//qDebug()<<"HoloLensCameratoHoloLens"<<HoloLensCameratoHoloLens;



 this->HoloLensToHoloLensMarkerMatrix=HoloLensToHoloLensMarker;
  this->KinectMarkerToKinectMatrix=KinectMarkerToKinect;
}
void Transform::poindCloudRegiMatProc(const QMatrix4x4& mat)
{
    qDebug() << "Transform received RegiMat:" << mat;
    pCRead=true;
    this->poindCloudRegiMat = mat;
}

void Transform::modelCalc()
{
    //发送的矩阵是result
    //还没实现将配准结果导入矩阵，研究下
    QMatrix4x4 result;
    QMatrix4x4 KinectMarkerToNDI;
    QMatrix4x4 HoloLensMarkerToNDI;
    QMatrix4x4 Registration;
    QMatrix4x4 ModelToHoloLens;
    QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
    QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
    QMatrix4x4 HoloLensToWorld;
    QMap<int,QVector3D> KinectMarkers;
    QMap<int,QVector3D> HoloLensMarkers;
    //get the markers' location of Kinect
    LoadCalibrationMatrix();
    if(existTools.contains(NdiTool("Kinect"))){
        int index = existTools.indexOf(NdiTool("Kinect"));
       KinectMarkers=existTools[index].getIndexAndCoordinate();
       KinectMarkerToNDI=SetCoordination1(KinectMarkers);
       qDebug()<<"kinectMarkertoNdi"<<KinectMarkerToNDI;
    }
    //get the markers' location of HoloLens
    if(existTools.contains(NdiTool("HoloLens"))){
        int index = existTools.indexOf(NdiTool("HoloLens"));
      HoloLensMarkers=existTools[index].getIndexAndCoordinate();
      // HoloLensMarkerToNDI=SetCoordination(HoloLensMarkers);
       HoloLensMarkerToNDI=SetCoordination1(HoloLensMarkers);
       qDebug()<<"HoloLensMarkerToNDI"<<HoloLensMarkerToNDI;
    }
    //get the matrixs of pointcloud registration
    if(pCRead==true)
         Registration=poindCloudRegiMat;
    else {
        getRegiMat();
    }
    // get the HololensLocalToWorld matrix，并把左手系换成右手系
    HoloLensToWorld=holoMat;
    HoloLensToWorld(0,2)=-HoloLensToWorld(0,2);
    HoloLensToWorld(1,2)=-HoloLensToWorld(1,2);
    HoloLensToWorld(2,0)=-HoloLensToWorld(2,0);
    HoloLensToWorld(2,1)=-HoloLensToWorld(2,1);
    HoloLensToWorld(2,3)=-HoloLensToWorld(2,3);


    //result
    result=HoloLensToWorld*HoloLensToHoloLensMarker.inverted()*HoloLensMarkerToNDI.inverted()*KinectMarkerToNDI*KinectMarkerToKinect.inverted();
    this->modelResultLastTimeMat=result;
    //右手系换左手系给HoloLens发送
    qDebug() << "beforemodelResultMat:" << result;
    result(0,2)=-result(0,2);
    result(1,2)=-result(1,2);
    result(2,0)=-result(2,0);
    result(2,1)=-result(2,1);
    result(2,3)=-result(2,3);
    qDebug() << "modelResultMat:" << result;

    //write last time modelResultMat

    this->matrixList.clear();
    this->matrixList.push_back(result);
}

void Transform::calibrationNeedleCalc()
{
    //发送的矩阵是result
    QMatrix4x4 result;
    QMatrix4x4 calibrationNeedleToNDI;
    QMatrix4x4 HoloLensToWorld;
    QMatrix4x4 HoloLensMarkerToNDI;
    QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
   // QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
    LoadCalibrationMatrix();
    // get the HololensLocalToWorld matrix，并把左手系换成右手系
    HoloLensToWorld=holoMat;
    HoloLensToWorld(0,2)=-HoloLensToWorld(0,2);
    HoloLensToWorld(1,2)=-HoloLensToWorld(1,2);
    HoloLensToWorld(2,0)=-HoloLensToWorld(2,0);
    HoloLensToWorld(2,1)=-HoloLensToWorld(2,1);
    HoloLensToWorld(2,3)=-HoloLensToWorld(2,3);

     //get the markers' location of Needle
    QMap<int,QVector3D> calibrationNeedleMarkers;
    if(existTools.contains(NdiTool("CalibrationNeedle"))){
        int index = existTools.indexOf(NdiTool("CalibrationNeedle"));
       calibrationNeedleMarkers=existTools[index].getIndexAndCoordinate();
       calibrationNeedleToNDI=SetCoordination2(calibrationNeedleMarkers);
    }
    this->calibritionNeedleLastTimeMat=calibrationNeedleToNDI;
    qDebug() << "calibrationNeedleToNDIMat:" << calibrationNeedleToNDI;

    //get the markers' location of HoloLens
    QMap<int,QVector3D> HoloLensMarkers;
    if(existTools.contains(NdiTool("HoloLens"))){
        int index = existTools.indexOf(NdiTool("HoloLens"));
      HoloLensMarkers=existTools[index].getIndexAndCoordinate();
       HoloLensMarkerToNDI=SetCoordination1(HoloLensMarkers);
    }
    qDebug() << "HoloLensMarkerToNDI:" << HoloLensMarkerToNDI;
     //result
     result=HoloLensToWorld*HoloLensToHoloLensMarker.inverted()*HoloLensMarkerToNDI.inverted()*calibrationNeedleToNDI;
     this->calibritionNeedleResultLastTimeMat=result;
     //右手系换左手系给HoloLens发送
     result(0,2)=-result(0,2);
     result(1,2)=-result(1,2);
     result(2,0)=-result(2,0);
     result(2,1)=-result(2,1);
     result(2,3)=-result(2,3);
     //qDebug() << "calibrationNeedleResultMat:" << result;
     //write last time calibrationNeedleResultMat

     this->matrixList.clear();
     this->matrixList.push_back(result);
}
void Transform::reviseMatrixCalc()
{
    //发送的矩阵是reviseModel和reviseCalibrationNeedle
     QMap<int,QVector3D> calibrationNeedleMarkers;
     QMatrix4x4 calibrationNeedleToNDI;
     QMatrix4x4 LastTimeCalibrationNeedleToNDI;
     QMatrix4x4 reviseMatrix;
     QMatrix4x4 HoloLensMarkerToNDI;
     QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
     QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
     QMatrix4x4 HoloLensToWorld;
     QMatrix4x4 LastTimeCalibrationNeedleResult=calibritionNeedleResultLastTimeMat;
     QMatrix4x4 LastTimeModelResult=modelResultLastTimeMat;
     QMatrix4x4 result;
     QMatrix4x4 reviseModel;
     QMatrix4x4 reviseCalibrationNeedle;
     LoadCalibrationMatrix();
     // get the HololensLocalToWorld matrix，并把左手系换成右手系
     HoloLensToWorld=holoMat;
     HoloLensToWorld(0,2)=-HoloLensToWorld(0,2);
     HoloLensToWorld(1,2)=-HoloLensToWorld(1,2);
     HoloLensToWorld(2,0)=-HoloLensToWorld(2,0);
     HoloLensToWorld(2,1)=-HoloLensToWorld(2,1);
     HoloLensToWorld(2,3)=-HoloLensToWorld(2,3);
     //get the markers' location of Needle last time
     LastTimeCalibrationNeedleToNDI=calibritionNeedleLastTimeMat;
     //get the markers' location of HoloLens
     QMap<int,QVector3D> HoloLensMarkers;
     if(existTools.contains(NdiTool("HoloLens"))){
          int index = existTools.indexOf(NdiTool("HoloLens"));
        HoloLensMarkers=existTools[index].getIndexAndCoordinate();
        HoloLensMarkerToNDI=SetCoordination1(HoloLensMarkers);
     }
     //get the markers' location of Needle this time
     if(existTools.contains(NdiTool("calibrationNeedle"))){
         int index = existTools.indexOf(NdiTool("calibrationNeedle"));
        calibrationNeedleMarkers=existTools[index].getIndexAndCoordinate();
        calibrationNeedleToNDI=SetCoordination1(calibrationNeedleMarkers);
     }
     //result
      result=HoloLensToWorld*HoloLensToHoloLensMarker.inverted()*HoloLensMarkerToNDI.inverted()*calibrationNeedleToNDI;

     //reviseMatrix
     reviseMatrix=result*LastTimeCalibrationNeedleResult.inverted();
     //result of new modelMatrix
     reviseModel=reviseMatrix*LastTimeModelResult;
     reviseCalibrationNeedle=result;
     //右手系换左手系给HoloLens发送
      reviseCalibrationNeedle(0,2)=-reviseCalibrationNeedle(0,2);
      reviseCalibrationNeedle(1,2)=-reviseCalibrationNeedle(1,2);
      reviseCalibrationNeedle(2,0)=-reviseCalibrationNeedle(2,0);
      reviseCalibrationNeedle(2,1)=-reviseCalibrationNeedle(2,1);
      reviseCalibrationNeedle(2,3)=-reviseCalibrationNeedle(2,3);
      reviseModel(0,2)=-reviseModel(0,2);
      reviseModel(1,2)=-reviseModel(1,2);
      reviseModel(2,0)=-reviseModel(2,0);
      reviseModel(2,1)=-reviseModel(2,1);
      reviseModel(2,3)=-reviseModel(2,3);
      qDebug() << "reviseModelResultMat:" << reviseModel;
      qDebug() << "reviseCalibrationNeedleResultMat:" << reviseCalibrationNeedle;
     this->matrixList.clear();
     this->matrixList.push_back(reviseModel);
     this->matrixList.push_back(reviseCalibrationNeedle);
}

void Transform::boneDrillCalc()
{
    //发送的矩阵是result
    QMatrix4x4 boneDrillToNDI;
    QMatrix4x4 HoloLensToHoloLensMarker=HoloLensToHoloLensMarkerMatrix;
   // QMatrix4x4 KinectMarkerToKinect=KinectMarkerToKinectMatrix;
    QMatrix4x4 HoloLensToWorld;
    QMatrix4x4 HoloLensMarkerToNDI;
    QMatrix4x4  result;
    LoadCalibrationMatrix();
    // get the HololensLocalToWorld matrix，并把左手系换成右手系
    HoloLensToWorld=holoMat;
    HoloLensToWorld(0,2)=-HoloLensToWorld(0,2);
    HoloLensToWorld(1,2)=-HoloLensToWorld(1,2);
    HoloLensToWorld(2,0)=-HoloLensToWorld(2,0);
    HoloLensToWorld(2,1)=-HoloLensToWorld(2,1);
    HoloLensToWorld(2,3)=-HoloLensToWorld(2,3);
    //get the markers' location of boneDrillCalc
    QMap<int,QVector3D> BoneDrillMarkers;
    if(existTools.contains(NdiTool("BoneDrill"))){
        int index = existTools.indexOf(NdiTool("BoneDrill"));
       BoneDrillMarkers=existTools[index].getIndexAndCoordinate();
       boneDrillToNDI=SetCoordination2(BoneDrillMarkers);
    }
    //get the markers' location of HoloLens
    QMap<int,QVector3D> HoloLensMarkers;
    if(existTools.contains(NdiTool("HoloLens"))){
         int index = existTools.indexOf(NdiTool("HoloLens"));
        HoloLensMarkers=existTools[index].getIndexAndCoordinate();
        HoloLensMarkerToNDI=SetCoordination1(HoloLensMarkers);
     }
     //result
     result=HoloLensToWorld*HoloLensToHoloLensMarker.inverted()*HoloLensMarkerToNDI.inverted()*boneDrillToNDI;
     //右手系换左手系给HoloLens发送
     result(0,2)=-result(0,2);
     result(1,2)=-result(1,2);
     result(2,0)=-result(2,0);
     result(2,1)=-result(2,1);
     result(2,3)=-result(2,3);
     qDebug() << "boneDrillModelResultMat:" << result;
     this->matrixList.clear();
     this->matrixList.push_back(result);
}
void Transform::holoMatrixProc(const QMatrix4x4& holoMat)
{
    this->holoMat=holoMat;
    //qDebug() << "HoloLens send:" << holoMat;
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
         calibrationNeedleCalc();
        //boneDrillCalc();
        emit readyForHololens(currentCommand, matrixList);
        break;
    default:
        break;
    }
}
