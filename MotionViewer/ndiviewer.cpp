#include "ndiviewer.h"
#include "ui_ndiviewer.h"
#include <QDebug>

NdiViewer::NdiViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiViewer)
{
    ui->setupUi(this);
    init();
    QList<QVector3D> markers;
    markers.push_back(QVector3D(1.1,2.1,3.1));
    markers.push_back(QVector3D(2.2,4.4,5.9));
    markers.push_back(QVector3D(5.6,7.8,5.4));

    refreshMarkersView(markers);

    refreshMatrixView(virtualTransformMatrix);

}

NdiViewer::~NdiViewer()
{
    delete ui;
}

void NdiViewer::init()
{
    ui->tblMarkersView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblToolMarkers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tblToolMarkers->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //set comboBox Functions
    //    QStringList functionLists;
    //    functionLists << tr("1. model") << tr("2. calibration needle") << tr("3. revise matrix") << tr("4. bone drill") ;
    //    ui->cmbSteps->addItems(functionLists);
}

void NdiViewer::refreshMarkersView(QList<QVector3D> data)
{

    //    ui->tblMarkersView->horizontalHeader()->setStretchLastSection(true);

    int count = data.count();

    ui->tblMarkersView->setRowCount(count);

    for(int i = 0; i< count; i++){
        QVector3D marker = data.at(i);
        ui->tblMarkersView->setItem(i,0,new QTableWidgetItem(QString::number(static_cast<double>(marker.x()))));
        ui->tblMarkersView->setItem(i,1,new QTableWidgetItem(QString::number(static_cast<double>(marker.y()))));
        ui->tblMarkersView->setItem(i,2,new QTableWidgetItem(QString::number(static_cast<double>(marker.z()))));
    }
}


void NdiViewer::refreshMatrixView(QMatrix4x4 mat)
{
    ui->tblToolMarkers->setRowCount(4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            ui->tblToolMarkers->setItem(i,j, new QTableWidgetItem(QString::number(static_cast<double>(mat(i,j)))));
        }
    }

}

void NdiViewer::dataProc(QList<QVector3D> data)
{
    QMap<QString,QList<QVector3D>> freshtool;
    qDebug() << tr("Coordinate is received by NdiViewer, value is: ") << data;
    refreshMarkersView(data);
    freshtool=getToolsNumAndPose(data);

    emit readyForRegistrate(freshtool); //emit signal to registrate

}

void NdiViewer::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(event);
    }
}

void NdiViewer::on_btnExec_clicked()
{
    const int index = ui->cmbSteps->currentIndex();
    switch (index) {
    case 0://step 1:model
        virtualTransformMatrix = getVirtualTransformMatrix();
        refreshMatrixView(virtualTransformMatrix);
        break;
    case 1://step 2:calibration needle
        realTransformMatrix = getRealTransformMatrix();
        refreshMatrixView(realTransformMatrix);
        break;
    case 2://step 3:revise matrix
        calibrationMatrix = getCalibrationMatrix();
        refreshMatrixView(calibrationMatrix);
        break;
    case 3: //step 4:bone drill
        break;
    default:
        break;
    }
}

void NdiViewer::on_cmbSteps_currentIndexChanged(int index)
{
    switch (index) {
    case 0://step 1
        refreshMatrixView(virtualTransformMatrix);
        break;
    case 1://step 2
        refreshMatrixView(realTransformMatrix);
        break;
    case 2://step 3
        refreshMatrixView(calibrationMatrix);
        break;
    default:
        break;
    }
}

QMap<QString,QList<QVector3D>> NdiViewer::getToolsNumAndPose(QList<QVector3D> data)
{
    QString toolname;
    QMap<QString,QList<QVector3D>> detectedTool;
    QList<QVector3D> zuobiao;
    QMap<QString,int> toolNameNumber;
    double distance[20];
    //    int keyvalue;
    // float a,b,c;
    int datacount = data.count();
    if ((datacount>2)&&(datacount<20))
    {
        for (int  i= 0; i< datacount; i++)
        {
            for(int j = 0; j< datacount; j++)
            {
                float a,b,c;
                a=data[i].x();
                b=data[i].y();

                c=data[i].z();
                distance[j]= sqrt(pow((data[i].x() - data[j].x()), 2) + pow((data[i].y() - data[j].y()), 2) + pow((data[i].z() - data[j].z()), 2));
            }
            toolname="";
            toolNameNumber=judgeTool(distance,toolname,datacount);
            if (toolname!="")
            {

                if(detectedTool.contains(toolname))
                {
                    zuobiao=detectedTool[toolname];
                    zuobiao.push_back(QVector3D(data[i].x(),data[i].y(),data[i].z()));
                    detectedTool[toolname]=zuobiao;
                }
                else
                {
                    zuobiao.push_back(QVector3D(data[i].x(),data[i].y(),data[i].z()));
                    detectedTool.insert(toolname,zuobiao);

                }
                qDebug() << tr("Find tools: ") << toolname<<tr("Coordinate is: ")<< zuobiao;
            }
        }
    }


    //   else
    //       detectedTool.insert("wu",data);
    return detectedTool;
}
QMap<QString,int> NdiViewer::judgeTool(double *dis,  QString &toolName,int &count)
{
    QMap<QString,QList<float>> toolNameSize;
    QMap<QString,int> toolNameNumber;
    QList<float> toolSize;
    int numofpoints;
    bool isToolOrNot;
    //double *distance=dis;
    //加一个读取配置的函数，赋值toolNameSize
    toolNameSize=getToolDefination();
    //返回值是QMap<QString,QList<float>>
    QMapIterator<QString,QList<float>> qq(toolNameSize);
    while(qq.hasNext()){
        toolSize=qq.next().value();
        numofpoints=toolSize.count();
        isToolOrNot=isTool(dis,toolSize,count) ;
        if(isToolOrNot==true)
        {
            toolName=qq.key();
            toolNameNumber.insert(toolName,count);
            break;
        }

    }

    return toolNameNumber;
}
QMap<QString,QList<float>>NdiViewer::getToolDefination()
{
    QMap<QString,QList<float>>ToolDefination;
    QList<float> Tool1;
    QList<float> Tool2;
    QList<float> Tool3;
    QList<float> Tool4;
    Tool1.push_back(137);
    Tool1.push_back(103);
    Tool1.push_back(88);
    Tool1.push_back(61);
    Tool1.push_back(56);
    Tool1.push_back(50);
    Tool2.push_back(150);
    Tool2.push_back(117);
    Tool2.push_back(105);
    Tool2.push_back(66);
    Tool2.push_back(59);
    Tool2.push_back(51);
    Tool3.push_back(88);
    Tool3.push_back(73);
    Tool3.push_back(65);
    Tool3.push_back(60);
    Tool3.push_back(54);
    Tool3.push_back(50);
    Tool4.push_back(150);
    Tool4.push_back(116);
    Tool4.push_back(105);
    Tool4.push_back(66);
    Tool4.push_back(60);
    Tool4.push_back(51);
    ToolDefination.insert("Needle",Tool1);
    ToolDefination.insert("HoloLens",Tool2);
    ToolDefination.insert("BoneDrill",Tool3);
    ToolDefination.insert("Kinect",Tool4);
    //加一个读取文本的东西
    return ToolDefination;
}
bool NdiViewer::isTool(double *distance,QList<float> toolSize,int &count)
{
    int numberOfMarkers;
    double err=2;
    int q= 0;
    double point=0;
    bool istool=false;
    numberOfMarkers=toolSize.count();
    for(int i=0;i<count;i++)
    {
        QListIterator<float>m(toolSize);
        while(m.hasNext())
        {
            point=m.next();
            if(((point-err)<distance[i])&&(distance[i]<(point+err)))
            {
                q=q+1;
                break;
            }
        }
        if (q==3)//这个地方只针对四个点的工具，到时候读tooldefination的时候定义一个类获取点的个数
        {
            istool=true;
            break;
        }
    }


    return istool;
}
QMatrix4x4 NdiViewer::getVirtualTransformMatrix()
{
    QMatrix4x4 mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}

QMatrix4x4 NdiViewer::getRealTransformMatrix()
{
    QMatrix4x4 mat(1.1,2.2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}

QMatrix4x4 NdiViewer::getCalibrationMatrix()
{
    QMatrix4x4 mat(1.5,2.7,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}
