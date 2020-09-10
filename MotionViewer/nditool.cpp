#include "nditool.h"

NdiTool::NdiTool()
{

}

NdiTool::NdiTool(const QString &name) : name(name)
{
}

bool NdiTool::operator==(const NdiTool &t) const
{
    if(this->name == t.name)
        return true;
    return false;
}

void NdiTool::setMarkersDistances(const QVector<QVector<float> > &markerDistances)
{
    this->markerDistances = markerDistances;
}

void NdiTool::addMarkerDistance(const QVector<float> &markerDistance)
{
    this->markerDistances.push_back(markerDistance);
}

QVector<QVector<float> > NdiTool::getMarkersDistances()
{
    return this->markerDistances;
}

void NdiTool::setMarkers(QVector<QVector3D>& markers)
{
    this->markers = markers;
}

void NdiTool::addMarker(QVector3D marker)
{
    this->markers.push_back(marker);
}

void NdiTool::insertIndexAndCoordinate(int index, QVector3D coordinate)
{
    coordinates[index] = coordinate;
}
QMap<int,QVector3D> NdiTool::getIndexAndCoordinate()
{
    return this->coordinates;
}

void NdiTool::setModelPath(const QString &path)
{
    modelPath = path;
}

void NdiTool::setModelScale(float s)
{
    scale = s;
}

void NdiTool::setCoordinateMethod(int method)
{
    coordinateMethod = method;
}

QMatrix4x4 NdiTool::SetCoordination(const QMap<int, QVector3D> &markers, int method)
{
    //FOR SU SHUN
    return QMatrix4x4();

}

QMatrix4x4 NdiTool::SetCoordination1(const QMap<int, QVector3D> &markers)
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

QMatrix4x4 NdiTool::SetCoordination2(const QMap<int, QVector3D> &markers)
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
//   qDebug()<<"Toolcoordination1Matrix"<<mat;
   return mat;
}

