#include "nditool.h"

NdiTool::NdiTool()
{

}

NdiTool::NdiTool(QString name) : name(name)
{
}

bool NdiTool::operator==(const NdiTool &t)
{
    if(this->name == t.name)
        return true;
    return false;
}

void NdiTool::setMarkersDistances(QList<QList<float> > markerDistances)
{
    this->markerDistances = markerDistances;
}

QList<QList<float> > NdiTool::getMarkersDistances()
{
    return this->markerDistances;
}

void NdiTool::setMarkers(QList<QVector3D> markers)
{
    this->markers = markers;
}

void NdiTool::insertIndexAndCoordinate(int index, QVector3D coordinate)
{
    coordinates[index] = coordinate;
}
QMap<int,QVector3D> NdiTool::getIndexAndCoordinate()
{
    return this->coordinates;
}

