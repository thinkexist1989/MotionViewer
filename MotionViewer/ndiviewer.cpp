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
    ui->tblMatrixView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblMatrixView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //set comboBox Functions
    QStringList functionLists;
    functionLists << tr("1. Virtual Tool Pose") << tr("2. Actual Tool Pose") << tr("3. Update Calibration Matrix");
    ui->cmbSteps->addItems(functionLists);
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
    ui->tblMatrixView->setRowCount(4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            ui->tblMatrixView->setItem(i,j, new QTableWidgetItem(QString::number(static_cast<double>(mat(i,j)))));
        }
    }
}

void NdiViewer::dataProc(QList<QVector3D> data)
{
    qDebug() << tr("Coordinate is received by NdiViewer, value is: ") << data;
    refreshMarkersView(data);
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
    case 0://step 1
        virtualTransformMatrix = getVirtualTransformMatrix();
        refreshMatrixView(virtualTransformMatrix);
        break;
    case 1://step 2
        realTransformMatrix = getRealTransformMatrix();
        refreshMatrixView(realTransformMatrix);
        break;
    case 2://step 3
        calibrationMatrix = getCalibrationMatrix();
        refreshMatrixView(calibrationMatrix);
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
    int keyvalue;
    int datacount = data.count();
    for (int  i= 0; i< datacount; i++){
        for(int j = 0; j< datacount; i++){
            distance[j]= sqrt(pow((data[i].x() - data[j].x()), 2) + pow((data[i].y() - data[j].y()), 2) + pow((data[i].z() - data[j].z()), 2));
        }
        toolname="";
        toolNameNumber=judgeTool(distance,toolname,datacount);
        if (toolname!="")
        {
           QMap<QString,QList<QVector3D>>::Iterator m;
            m=detectedTool.find(toolname);
            zuobiao=m.value();
            zuobiao.push_back(QVector3D(data[i].x(),data[i].y(),data[i].z()));
            m.value()=zuobiao;
            if(!detectedTool.contains(toolname))
            {
                zuobiao.push_back(QVector3D(data[i].x(),data[i].y(),data[i].z()));
                detectedTool.insert(toolname,zuobiao);
            }
        }
       }

  QMap<QString,QList<QVector3D>> qmap;
qmap.insert("tool1",data);
return qmap;
}
QMap<QString,int> NdiViewer::judgeTool(double *dis,  QString &toolName,int &count)
{
    QMap<QString,QList<float>> toolNameSize;
    QMap<QString,int> toolNameNumber;
    QList<float> toolSize;
    int numofpoints;
    bool isToolOrNot;
    double *distance=dis;
//加一个读取配置的函数，赋值toolNameSize    
    //toolNameSize=getToolDefination()
    //返回值是QMap<QString,QList<float>>
    QMapIterator<QString,QList<float>>i(toolNameSize);
    while(i.hasNext()){
        toolSize=i.next().value();
        numofpoints=toolSize.count();
        isToolOrNot=isTool(distance,toolSize,count) ;
        if(isToolOrNot)
        {
            toolName=i.next().key();
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
ToolDefination.insert("CalibrationNeedle",Tool1);
ToolDefination.insert("HoloLens",Tool2);
ToolDefination.insert("BoneDrill",Tool3);
//加一个读取文本的东西
return ToolDefination;
}
bool NdiViewer::isTool(double *distance,QList<float> toolSize,int &count)
{
    int numberOfMarkers;
    double err=2;
    int q= 0;
    bool istool=false;
    numberOfMarkers=toolSize.count();
    for(int i=0;i<count;i++)
   {
        QListIterator<float>m(toolSize);
        while(m.hasNext())
        {

          if(((m.next()-err)<distance[i])&&(distance[i]<(m.next()+err)))
          {
                   q=q+1;
                   break;
          }
        }
        if (q==numberOfMarkers)
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
