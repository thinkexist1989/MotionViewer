#include "xmlparser.h"
#include <QFile>
#include <QDebug>

XmlParser::XmlParser(QObject *parent) : QObject(parent)
{

}

QVector<NdiTool> XmlParser::getToolsByCoordinatesFromXml(QString fileName)
{
    int i = 0;
    QVector<NdiTool> tools;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        qDebug() << tr("XmlParser: Can not Open Xml File");
        return tools;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        qDebug() << tr("XmlParser: Can not Read Xml File");
        file.close();
        return tools;
    }
    file.close();

    QDomElement root = doc.documentElement(); //return root node
    if(root.nodeName().toLower() != ("tooldefinition")){
        qDebug() << tr("XmlParser: The xml must contain <ToolDefinition> as document root ");
        return tools;
    }
    QDomNode toolNode = root.firstChild(); // get first tool
    while(!toolNode.isNull()){
        if(toolNode.isElement()){
            QDomElement e = toolNode.toElement();
            if(e.tagName().toLower() == "tool"){
                qDebug() << tr("XmlParser: Tools ").toStdString().c_str() << ++i << tr("=>").toStdString().c_str() << e.attribute("name");
                NdiTool tool(e.attribute("name"));
                tool.setModelPath(e.attribute("model")); //保存模型文件的名称
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
    qDebug() << tr("Total find").toStdString().c_str() << tools.count() << tr("tools").toStdString().c_str();
    return tools;
}

QVector<NdiTool> XmlParser::getToolsByDistancesFromXml(QString fileName)
{
    int i = 0;
    QVector<NdiTool> tools;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        qDebug() << tr("XmlParser: Can not Open Xml File");
        return tools;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        qDebug() << tr("XmlParser: Can not Read Xml File");
        file.close();
        return tools;
    }
    file.close();

    QDomElement root = doc.documentElement(); //return root node
    if(root.nodeName().toLower() != ("tooldefinition")){
        qDebug() << tr("XmlParser: The xml must contain <ToolDefinition> as document root ");
        return tools;
    }
    QDomNode toolNode = root.firstChild(); // get first tool
    while(!toolNode.isNull()){
        if(toolNode.isElement()){
            QDomElement e = toolNode.toElement();
            if(e.tagName().toLower() == "tool"){
                qDebug() << tr("XmlParser: Tools ").toStdString().c_str() << ++i << tr("=>").toStdString().c_str() << e.attribute("name");
                NdiTool tool(e.attribute("name"));
                tool.setModelPath(e.attribute("model")); //保存模型文件的名称
                tool.setModelScale(e.attribute("scale").toFloat()); //保存模型缩放比例
                tool.setCoordinateMethod(e.attribute("coordination").toInt()); //保存设置坐标系方法
                QDomNodeList markerNodes = e.childNodes();
                for (int i = 0; i < markerNodes.count(); i++) {
                    QDomNode markerNode = markerNodes.at(i);
                    if(markerNode.isElement()){
                        QDomElement e = markerNode.toElement();
                        if(e.tagName().toLower() == "marker"){
                            QString alldists = e.attribute("dis");
                            QStringList strList = alldists.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
                            QVector<float> dists;
                            foreach(QString dis, strList){
                                dists.push_back(dis.toFloat());
                            }
                            tool.addMarkerDistance(dists);
                        }
                    }
                }
                tools.push_back(tool);
            }
        }
        toolNode = toolNode.nextSibling();
    }
    qDebug() << tr("Total find").toStdString().c_str() << tools.count() << tr("tools").toStdString().c_str();
    return tools;
}

