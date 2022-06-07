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
        qDebug() << tr("XmlParser: Can not Open Xml File--at line 16");
        return tools;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        qDebug() << tr("XmlParser: Can not Read Xml File---at line 22");
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
    //测试代码
    /**
     * 测试结果：发现程序运行时文件所在的当前文件的位置是：C:\Users\Ausland\Documents\GitHub\MotionViewer\build-MotionViewer-Desktop_Qt_5_12_8_MSVC2017_64bit-Debug
     * 在项目的Debug目录里面
     * ../MotionViewer/tooldef.xml
     */
//    QFile file("../MotionViewer/tooldef.xml");//当前执行路径文件在：C:\Users\Ausland\Documents\GitHub\MotionViewer\build-MotionViewer-Desktop_Qt_5_12_8_MSVC2017_64bit-Debug
//    QFile file("../MotionViewer/tooldef-hololen2-2.xml");
    QFile file("../MotionViewer/tooldef-hololen2.xml");
    if(!file.open(QIODevice::ReadOnly))//以读的方式打开文件
        qDebug()<<file.errorString();

    QByteArray buf = file.readAll();//读取文件所有数据
//    qDebug()<<"buf: "<<buf;

    file.close();//关闭文件


    int i = 0;
    QVector<NdiTool> tools;
    //目前可以读取到当前的文件名
    qDebug() << tr("the fileName is ")+fileName;
//    QFile file(fileName);

    if(!file.open(QFile::ReadOnly)){
        //以后用这个输出错误，可以方便我们定位问题原因并去解决他
        qDebug()<<file.errorString();
        qDebug() << tr("XmlParser: Can not Open Xml File--at line 69");
        return tools;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        qDebug()<<file.errorString();
        qDebug() << tr("XmlParser: Can not Read Xml File--at line 75");
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
                            qDebug() << dists;
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

