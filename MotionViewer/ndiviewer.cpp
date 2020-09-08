#include "ndiviewer.h"
#include "ui_ndiviewer.h"
#include <QDebug>
#include <QListIterator>
#include <QVectorIterator>
#include <iterator>
#include <QFileDialog>

NdiViewer::NdiViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NdiViewer)
{
    ui->setupUi(this);
    init();

//    QList<QVector3D> markers;
//    markers.push_back(QVector3D(1.1,2.1,3.1));
//    markers.push_back(QVector3D(2.2,4.4,5.9));
//    markers.push_back(QVector3D(5.6,7.8,5.4));

    //refreshMarkersView(markers);

    //refreshMatrixView(virtualTransformMatrix);

//    QList<QVector3D> list1;
//    list1 << QVector3D(1.1,2.1,3.1) << QVector3D(1.1,2.1,3.1);

}

NdiViewer::~NdiViewer()
{
    delete ui;
}

void NdiViewer::init()
{
    ui->tblMarkersView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblToolMarkers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    getToolDefination();
    //ui->tblToolMarkers->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //set comboBox Functions
    //    QStringList functionLists;
    //    functionLists << tr("1. model") << tr("2. calibration needle") << tr("3. revise matrix") << tr("4. bone drill") ;
    //    ui->cmbSteps->addItems(functionLists);
}

void NdiViewer::refreshMarkersView(QList<QVector3D> markersData)
{

    //    ui->tblMarkersView->horizontalHeader()->setStretchLastSection(true);

    int count = markersData.count();

    ui->tblMarkersView->setRowCount(count);

    for(int i = 0; i< count; i++){
        QVector3D marker = markersData.at(i);
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

void NdiViewer::refreshToolView(QList<NdiTool> existtools)
{
    if(existtools.count() == 0){
        //return;
        ui->lstToolName->clear();
        ui->tblToolMarkers->clear();}
    int count = existtools.count();
    QStringList toolnames;
    for (int i = 0; i < count ; i++){
        toolnames << existtools[i].getName();
    }
    ui->lstToolName->clear();
    ui->lstToolName->addItems(toolnames);

    if(ui->lstToolName->currentRow() != -1){
        QString toolName = ui->lstToolName->currentItem()->text();
        on_lstToolName_currentTextChanged(toolName);
    }
    else {
        ui->lstToolName->setCurrentRow(0);
    }

}

void NdiViewer::refreshMarkersInTool(NdiTool tool)
{
    int count = tool.coordinates.count();
    ui->tblToolMarkers->setRowCount(count);
    for (int i = 0; i < count; i++) {
        if(tool.coordinates.contains(i)){
            QVector3D coordinate = tool.coordinates[i];
            ui->tblToolMarkers->setItem(i,0, new QTableWidgetItem(QString::number(static_cast<double>(coordinate.x()))));
            ui->tblToolMarkers->setItem(i,1, new QTableWidgetItem(QString::number(static_cast<double>(coordinate.y()))));
            ui->tblToolMarkers->setItem(i,2, new QTableWidgetItem(QString::number(static_cast<double>(coordinate.z()))));
        }

    }
}

void NdiViewer::dataProc(QList<QVector3D> data)
{
    //qDebug() << data.size()<< endl;
    //qDebug() << tr("Coordinate is received by NdiViewer, value is: ") << data;
    refreshMarkersView(data);
    existTools=getTools(data);
    /*
    if(existTools.count()>0){
        for(int i=0;i<existTools.count();i++)
        qDebug() << existTools[i].getName()<<endl;//显示检测出什么工具
    }
    else
    //qDebug() <<"no tools is detected"<<endl; //显示什么也没检测出来
    */
    refreshToolView(existTools);
    emit toolsReady(existTools);

//    if(!tools.isEmpty())
//    {
//        qDebug() <<"total detect"<<tools.length()<<"tool";
//        for (int i=0;i<tools.length();i++) {
//                         qDebug() << "detected tools name:" << tools[i].getName() << "count:" << tools[i].coordinates.count() ;
//                   }
//    }

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
        emit readyForTransform(HOLO_MODEL,existTools);//emit signal to registrate
        break;
    case 1://step 2:calibration needle
        emit readyForTransform(HOLO_CALI_NEEDLE,existTools);//emit signal to registrate
        break;
    case 2://step 3:revise matrix
        emit readyForTransform(HOLO_REVISE_MATRIX,existTools);//emit signal to registrate
        break;
    case 3: //step 4:bone drill
        emit readyForTransform(HOLO_BONE_DRILL,existTools);//emit signal to registrate
        break;
    default:
        break;
    }
}

void NdiViewer::on_cmbSteps_currentIndexChanged(int index)
{
    switch (index) {
    case 0://step 1:model
        break;
    case 1://step 2:calibration needle
        break;
    case 2://step 3:revise matrix
        break;
    case 3://step 4:bone drill
    default:
        break;
    }
}

QList<NdiTool> NdiViewer::getTools(QList<QVector3D> data)
{
    QList<NdiTool> detectedTools;
    QList<QVector3D> markers;
    QPair<QString,int> toolNameIndex;
    QVector<float> dists;
    int datacount = data.count();
    if ((datacount>2)&&(datacount<50))
    {
        for (int  i= 0; i< datacount; i++)
        {
            dists.clear();
            for(int j = 0; j< datacount; j++)
            {    
                float dis = sqrt(pow((data[i].x() - data[j].x()), 2)
                                 + pow((data[i].y() - data[j].y()), 2)
                                 + pow((data[i].z() - data[j].z()), 2));
                dists.push_back(dis);
            }
            toolNameIndex=judgeTool(dists);
            if (toolNameIndex.first != "")
            {

                NdiTool tool(toolNameIndex.first);
                if(detectedTools.contains(tool)){
                    int index = detectedTools.indexOf(tool);
                    detectedTools[index].insertIndexAndCoordinate(toolNameIndex.second, data[i]);
                }
                else {
                    tool.insertIndexAndCoordinate(toolNameIndex.second, data[i]);
                    detectedTools.push_back(tool);
                }
                //qDebug() << tr("Find tools: ") << tool.getName();
            }
        }
    }
    return detectedTools;
}

void NdiViewer::getRegiMat()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Poind Cloud Registration Matrix File"), QDir::currentPath(), "TXT (*.txt)");
    if(QFile::exists(fileName)){
        emit needRegiMat(fileName);
    }
}

QPair<QString, int> NdiViewer::judgeTool(QVector<float> dists)
{
    QPair<QString,int> toolNameIndex;
    //double *distance=dis;
    //加一个读取配置的函数，赋值toolNameSize
    //返回值是QMap<QString,QList<float>>
    QList<NdiTool>::iterator i;
    for (i = tools.begin(); i != tools.end(); i++) {
        NdiTool tool = *i;
        int index = -1;
        bool isToolOrNot = isTool(dists, tool, index);
        if(isToolOrNot==true)
        {
            toolNameIndex.first = tool.getName();
            toolNameIndex.second = index;
            break;
        }
    }

    return toolNameIndex;
}

void NdiViewer::getToolDefination()
{
//    NdiTool Tool1("calibrationNeedle");
//    NdiTool Tool2("HoloLens");
//    NdiTool Tool3("BoneDrill");
//    NdiTool Tool4("Kinect");

//    QList<QList<float>> Tool1markerDistances;
//    QList<float> Tool1p1; Tool1p1 << 88 << 56 << 50;
//    QList<float> Tool1p2; Tool1p2 << 137 << 103 << 50;
//    QList<float> Tool1p3; Tool1p3 << 61 << 103 << 56;
//    QList<float> Tool1p4; Tool1p4 << 137 << 88 << 61;
//    Tool1markerDistances << Tool1p1 << Tool1p2 << Tool1p3 << Tool1p4;
//    Tool1.setMarkersDistances(Tool1markerDistances);

//    QList<QList<float>> Tool2markerDistances;
//    QList<float> Tool2p1; Tool2p1 << 66 << 105 << 59;
//    QList<float> Tool2p2; Tool2p2 << 117 << 51 << 59;
//    QList<float> Tool2p3; Tool2p3 << 117 << 66 << 150;
//    QList<float> Tool2p4; Tool2p4 << 150 << 51 << 105;
//    Tool2markerDistances << Tool2p1 << Tool2p2 << Tool2p3 << Tool2p4;
//    Tool2.setMarkersDistances(Tool2markerDistances);

//    QList<QList<float>> Tool3markerDistances;
//    QList<float> Tool3p1; Tool3p1 << 60 << 88 << 50;
//    QList<float> Tool3p2; Tool3p2 << 73 << 54 << 50;
//    QList<float> Tool3p3; Tool3p3 << 65 << 88 << 55;
//    QList<float> Tool3p4; Tool3p4 << 60 << 73 << 65;
//    Tool3markerDistances << Tool3p1 << Tool3p2 << Tool3p3 << Tool3p4;
//    Tool3.setMarkersDistances(Tool3markerDistances);

//    QList<QList<float>> Tool4markerDistances;
//    QList<float> Tool4p1; Tool4p1 << 76 << 120 ;
//    QList<float> Tool4p2; Tool4p2 << 76 << 192 ;
//    QList<float> Tool4p3; Tool4p3 << 120 <<192 ;
//    Tool4markerDistances << Tool4p1 << Tool4p2 << Tool4p3 ;
//    Tool4.setMarkersDistances(Tool4markerDistances);

//    QList<NdiTool> tools;
//    tools << Tool1 << Tool2 << Tool3 << Tool4;

    //read tool definition from xml file
    QString fileName = QCoreApplication::applicationDirPath();
    qDebug()<<fileName<<endl;
    tools = XmlParser::getToolsByDistancesFromXml("../MotionViewer/tooldef.xml");
   // return tools;
}

bool NdiViewer::isTool(QVector<float> dists, NdiTool toolx, int &index)
{
    QList<QList<float>> tool = toolx.getMarkersDistances();
    float err=2;

    bool istool=false;
    int numberOfMarkers = tool.count();
    QList<QList<float>>::iterator i;
    for (i = tool.begin(); i != tool.end(); i++) {
        int q= 0;
        QList<float> markerDists = *i;
        QList<float>::iterator j;
        for (j = markerDists.begin(); j != markerDists.end(); j++) {
            float markerDis = *j;
            QVector<float>::iterator k;
            for (k = dists.begin(); k != dists.end(); k++) {
                float dist = *k;
                if(((markerDis-err)<dist)&&(dist<(markerDis+err)))
                {
                    q=q+1;
                    break;
                }
            }
            if (q == (numberOfMarkers-1))//这个地方只针对四个点的工具，到时候读tooldefination的时候定义一个类获取点的个数
            {
                istool = true;
                index = i - tool.begin();
                break;
            }
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
    QMatrix4x4 mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}

QMatrix4x4 NdiViewer::getCalibrationMatrix()
{
    QMatrix4x4 mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    return mat;
}

void NdiViewer::on_lstToolName_currentTextChanged(const QString &currentText)
{
    NdiTool temp(currentText);
    if(existTools.contains(temp)){
        int index = existTools.indexOf(temp);
        refreshMarkersInTool(existTools[index]);
    }
}
