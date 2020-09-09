#ifndef NDITOOL_H
#define NDITOOL_H

#include <QVector>
#include <QVector3D>
#include <QString>
#include <QMap>

class NdiTool
{
public:
    explicit NdiTool();
    NdiTool(QString name);

    bool operator==(const NdiTool &t) const; //overload operator== to judge if tool is equal

    QString getName() {return this->name;}
    void setMarkersDistances(QVector<QVector<float>> markerDistances); //set all markers' distances
    void addMarkerDistance(QVector<float> markerDistance); // add distance of one marker
    QVector<QVector<float>> getMarkersDistances();

    void setMarkers(QVector<QVector3D>&);
    void addMarker(QVector3D marker);

    QMap<int,QVector3D> coordinates;

    void insertIndexAndCoordinate(int index, QVector3D coordinate);
    QMap<int,QVector3D> getIndexAndCoordinate();
private:
    QString name;
    QVector<QVector3D> markers;
    QVector<QVector<float>> markerDistances;
    int count;

};

#endif // NDITOOL_H
