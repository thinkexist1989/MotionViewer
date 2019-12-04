#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QtXml>
#include <nditool.h>

class XmlParser : public QObject
{
    Q_OBJECT
public:
    explicit XmlParser(QObject *parent = nullptr);

    static QList<NdiTool> getToolsByCoordinatesFromXml(QString fileName);
    static QList<NdiTool> getToolsByDistancesFromXml(QString fileName);

signals:

public slots:
};

#endif // XMLPARSER_H
