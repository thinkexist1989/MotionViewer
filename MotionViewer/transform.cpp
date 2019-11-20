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

void Transform::poindCloudRegiMatProc(QMatrix4x4 mat)
{
    this->poindCloudRegiMat = mat;
}

void Transform::modelCalc()
{
    //get the markers' location of Kinect
    QMap<int,QVector3D> KinectMarkers;
    if(tools.contains(NdiTool("Kinect"))){
        int index = tools.indexOf(NdiTool("Kinect"));
       KinectMarkers=tools[index].getIndexAndCoordinate();
    }

    //get the markers' location of HoloLens

    //get the matrixs of pointcloud registration
}

void Transform::calibrationNeedleCalc()
{
     //get the markers' location of Needle
}

void Transform::reviseMatrixCalc()
{
     //get the markers' location of Needle last time

     //get the markers' location of Needle this time
}

void Transform::boneDrillCalc()
{
    //get the markers' location of boneDrillCalc

}

