#ifndef NDITOOL_H
#define NDITOOL_H

#include <QVector>
#include <QVector3D>
#include <QString>
#include <QMap>
#include <QMatrix4x4>

class NdiTool
{
public:
    explicit NdiTool();
    NdiTool(const QString& name);

    QString name;
    QString modelPath = "";
    float scale = 1.0f;
    int coordinateMethod = 2;

    bool operator==(const NdiTool &t) const; //overload operator== to judge if tool is equal

    QString getName() {return this->name;}
    void setMarkersDistances(const QVector<QVector<float>>& markerDistances); //set all markers' distances
    void addMarkerDistance(const QVector<float>& markerDistance); // add distance of one marker
    QVector<QVector<float>> getMarkersDistances();

    void setMarkers(QVector<QVector3D>&);
    void addMarker(QVector3D marker);

    QMap<int,QVector3D> coordinates;

    void insertIndexAndCoordinate(int index, QVector3D coordinate);
    QMap<int,QVector3D> getIndexAndCoordinate();

    void setModelPath(const QString &path);
    void setModelScale(float s);
    void setCoordinateMethod(int method);

    QMatrix4x4 SetCoordination(const QMap<int,QVector3D>& markers);
    QMatrix4x4 SetCoordination1(const QMap<int,QVector3D>& markers);
    QMatrix4x4 SetCoordination2(const QMap<int,QVector3D>& markers);
private:
    QVector<QVector3D> markers;
    QVector<QVector<float>> markerDistances;
    int count;
};

#endif // NDITOOL_H
