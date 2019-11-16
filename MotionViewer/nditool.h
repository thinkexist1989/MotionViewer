#ifndef NDITOOL_H
#define NDITOOL_H

#include <QObject>
#include <QList>
#include <QVector3D>
#include <QString>

class NdiTool : public QObject
{
    Q_OBJECT
public:
    explicit NdiTool(QObject *parent = nullptr);
private:
    QString name;
    QList<QVector3D> markers;

signals:

public slots:
};

#endif // NDITOOL_H
