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

void NdiTool::addMarkerDistance(QList<float> markerDistance)
{
    this->markerDistances.push_back(markerDistance);
}

QList<QList<float> > NdiTool::getMarkersDistances()
{
    return this->markerDistances;
}

void NdiTool::setMarkers(QList<QVector3D> markers)
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

