#include "nditool.h"

NdiTool::NdiTool()
{

}

NdiTool::NdiTool(QString name) : name(name)
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

