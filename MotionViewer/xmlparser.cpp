#include "xmlparser.h"
#include <QFile>
#include <QDebug>

XmlParser::XmlParser(QObject *parent) : QObject(parent)
{

}

QList<NdiTool> XmlParser::getToolsByCoordinatesFromXml(QString fileName)
{
    QList<NdiTool> tools;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        qDebug() << tr("Can not Open Xml File");
        return tools;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        qDebug() << tr("Can not Read Xml File");
        file.close();
        return tools;
    }
    file.close();

    QDomElement root = doc.documentElement(); //return root node
    if(!root.nodeName().toLower().contains("tooldefinition")){
        qDebug() << tr("The xml must contain <ToolDefinition> as document root ");
        return tools;
    }
    QDomNode toolNode = root.firstChild(); // get first tool
    while(!toolNode.isNull()){
        if(toolNode.isElement()){
            QDomElement e = toolNode.toElement();
            if(e.tagName().toLower() == "tool"){
                qDebug() << tr("Find Tools:") << e.attribute("name");
                NdiTool tool(e.attribute("name"));
                QDomNodeList markerNodes = e.childNodes();
                for (int i = 0; i < markerNodes.count(); i++) {
                    QDomNode markerNode = markerNodes.at(i);
                    if(markerNode.isElement()){
                        QDomElement e = markerNode.toElement();
                        if(e.tagName().toLower() == "marker"){
                            float x = e.attribute("x").toFloat();
                            float y = e.attribute("y").toFloat();
                            float z = e.attribute("z").toFloat();
                            tool.addMarker(QVector3D(x,y,z));
                        }
                    }
                }
                tools.push_back(tool);
            }
        }
        toolNode = toolNode.nextSibling();
    }

    return tools;
}

QList<NdiTool> XmlParser::getToolsByDistancesFromXml(QString fileName)
{
    QList<NdiTool> tools;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        qDebug() << tr("Can not Open Xml File");
        return tools;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        qDebug() << tr("Can not Read Xml File");
        file.close();
        return tools;
    }
    file.close();

    QDomElement root = doc.documentElement(); //return root node
    if(!root.nodeName().toLower().contains("tooldefinition")){
        qDebug() << tr("The xml must contain <ToolDefinition> as document root ");
        return tools;
    }
    QDomNode toolNode = root.firstChild(); // get first tool
    while(!toolNode.isNull()){
        if(toolNode.isElement()){
            QDomElement e = toolNode.toElement();
            if(e.tagName().toLower() == "tool"){
                qDebug() << tr("Find Tools:") << e.attribute("name");
                NdiTool tool(e.attribute("name"));
                QDomNodeList markerNodes = e.childNodes();
                for (int i = 0; i < markerNodes.count(); i++) {
                    QDomNode markerNode = markerNodes.at(i);
                    if(markerNode.isElement()){
                        QDomElement e = markerNode.toElement();
                        if(e.tagName().toLower() == "marker"){
                            float x = e.attribute("x").toFloat();
                            float y = e.attribute("y").toFloat();
                            float z = e.attribute("z").toFloat();
                            tool.addMarker(QVector3D(x,y,z));
                        }
                    }
                }
                tools.push_back(tool);
            }
        }
        toolNode = toolNode.nextSibling();
    }

    return tools;
}

