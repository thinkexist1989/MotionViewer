#include "transform.h"

Transform::Transform(QObject *parent) : QObject(parent)
{

}

void Transform::transformProc(QMap<QString, QList<QVector3D>> tools)
{
    //TODO: add transform code

    QList<QMatrix4x4> matrixList;
    QMatrix4x4 m1,m2;
    matrixList << m1 << m2;
    emit readyForHololens(0,matrixList);
}

void Transform::holoMatrixProc(QMatrix4x4 holoMat)
{

}

