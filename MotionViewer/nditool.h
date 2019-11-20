#ifndef NDITOOL_H
#define NDITOOL_H

#include <QList>
#include <QVector3D>
#include <QString>

class NdiTool
{
public:
    explicit NdiTool();
    NdiTool(QString name);

    bool operator==(const NdiTool &t); //overload operator== to judge if tool is equal

    QString getName() {return this->name;}
    void setMarkersDistances(QList<QList<float>> markerDistances);
    QList<QList<float>> getMarkersDistances();

    void setMarkers(QList<QVector3D>);

    QMap<int,QVector3D> coordinates;

    void insertIndexAndCoordinate(int index, QVector3D coordinate);
    QMap<int,QVector3D> getIndexAndCoordinate();
private:
    QString name;
    QList<QVector3D> markers;
    QList<QList<float>> markerDistances;
    int count;

};

#endif // NDITOOL_H
