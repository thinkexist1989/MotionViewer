#include "glviewer.h"
#include "ui_glviewer.h"
#include <QTimer>
#include <QPainter>

#include <QMouseEvent> //process mouse event
#include <QWheelEvent> //process wheel event
#include <QFileInfo>
#include <QtMath>
#include <xmlparser.h>


GLViewer::GLViewer(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::GLViewer)
{
    ui->setupUi(this);

    camera = new GLCamera();

}

GLViewer::~GLViewer()
{
    delete ui;
}

void GLViewer::drawShaded()
{
    setDrawMode(0);
}

void GLViewer::drawWireframe()
{
    setDrawMode(1);
}

void GLViewer::setDrawMode(int mode)
{
    drawMode = mode;
    update(); //refresh display
}

void GLViewer::setProjectionMode(int mode)
{
    projMode = mode;
    update();
}

void GLViewer::drawNodes()
{
    initializeOpenGLFunctions();

    for(auto& node : nodes)
    {
        QMatrix4x4 trans;
        trans.translate(node);
        nodeModel->draw(view, projection, camera->model*trans);
    }
}

void GLViewer::initializeGL()
{
    initializeOpenGLFunctions();

//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //default background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //default background

//    backdrop = new Backdrop();

    coordinate = new GLCoordinate();

    nodeModel = new GLModel(QFileInfo("./sphere.dae").absoluteFilePath().toStdString());

    ndiModel = new GLModel(QFileInfo("./ndi.stl").absoluteFilePath().toStdString());
    ndiModel->setColor(QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

    loadTools();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{update();});
    timer->start(10);

    /*** test nodes ***/
//    QTimer *genNodesTimer = new QTimer(this);
//    connect(genNodesTimer, &QTimer::timeout, this, [=]{
//        nodes.clear();
//        int sum = rand()%20;
//        for(int i = 0; i < sum; i++)
//            nodes << QVector3D((rand()%1000 -500.0)/1000.0, rand()%300/300.0, (rand()%1000 -500.0)/1000.0);
//    });
//    genNodesTimer->start(2000);

}

void GLViewer::paintGL()
{
    if(drawMode == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if(projMode == 1)
    {
        projection.setToIdentity(); // same as view
        projection.ortho(-1.0f*camera->zoom, 1.0f*camera->zoom, -1.0f*height()/(float)width()*camera->zoom, 1.0f*height()/(float)width()*camera->zoom, 0.1, 100);
//        projection.perspective(camera->zoom, (float)width()/height(), 0.1, 100);
    }
    else
    {
        projection.setToIdentity(); // same as view
        projection.perspective(camera->fov, (float)width()/height(), 0.1, 100);
    }

//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //default background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //default background

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); //enable depth test
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);


    /*** view and projection matrix ***/
    view = camera->getViewMatrix();

//    projection.setToIdentity(); // same as view
//    projection.ortho(0.0f, (float)width()/height()*fov, 0.0f, (float)fov, 0.1, 100); //正交投影


//    projection.perspective(camera->zoom, (float)width()/height(), 0.1, 100);

    /*** background ***/
//    backdrop->draw();

    /*** axis ***/
    QMatrix4x4 model;
    //画坐标系网格
    //coordinate->draw(view, projection, camera->model* model);


    /*** nodes ***/
//    nodes.clear();
//    nodes << QVector3D(0,0,0)<< QVector3D(-0.3,-0.3,-0.3) << QVector3D(0.3,-0.3,-0.3) << QVector3D(-0.3,0.3,-0.3) << QVector3D(-0.3,-0.3,0.3);
//    nodes << QVector3D((rand()%1000 -500.0)/1000.0, rand()%300/300.0, (rand()%1000 -500.0)/1000.0);
    //画节点
    //drawNodes();


    /*** ndi ***/
    model.scale(0.001);
    ndiModel->draw(view, projection, camera->model*model);

    /*** tools ***/
    for(auto& tool : existTools)
    {
        QMatrix4x4 model;
        model.scale(tool.scale);
        //qDebug() << tool.scale;
        model = tool.SetCoordination(tool.getIndexAndCoordinate()) * model;
       // qDebug()<<model<<endl;
        toolModels[tool.name]->draw(view, projection, camera->model* model);
      /*
        if(tool.name=="CalibrationNeedle"){
            qDebug()<<"111"<<endl;
            tipModel = new GLModel(QFileInfo("./sphere.dae").absoluteFilePath().toStdString());
            QMatrix4x4 tip;
            QMatrix4x4 tooltip;
            QVector3D tippos=QVector3D(-205.95f, 0.0f, 0.0f);
            tipModel->setColor(QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
            tip.translate(tippos);
            tooltip=model*tip;
            tooltip.scale(0.001);
            qDebug()<<tooltip<<endl;
            tipModel->draw(view, projection,camera->model*tooltip);
        }
        */
    }

    /*** status ***/
    QString s1 = QString("Status: Dectected %1 nodes.").arg(nodes.size());
    QString s2 = QString("        Phased %1 tools.").arg(existTools.size());
    QStringList ss; ss << s1 << s2;

    setStatus(ss);

    drawStatus(); //draw status text on viewport
}

void GLViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        _mouse_pos = event->pos(); //store the mouse pos when pressed
    }
    else if(event->button() == Qt::RightButton)
    {
        _mouse_pos = event->pos();
        setCursor(Qt::SizeAllCursor);
    }
    else if(event->button() == Qt::MidButton)
    {
        _mouse_pos = event->pos();
        setCursor(Qt::ClosedHandCursor); //set cursor style
    }
}

void GLViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton ||
        event->button() == Qt::RightButton || Qt::MidButton)
    {
        unsetCursor(); //reset cursor style
    }
}

void GLViewer::mouseMoveEvent(QMouseEvent *event)
{
    auto p = event->pos(); //current mouse pos
    auto d = p - _mouse_pos; //move distance between mouse pressed and current
//    qDebug() <<d;

    if(event->buttons() & Qt::LeftButton) //left button -> rotation
    {

        camera->cameraRotate(d);

//        qDebug() << QString("yaw: %1, tilt: %2").arg(yaw).arg(tilt).toStdString().c_str();
        update();
    }
    else if(event->buttons() & Qt::RightButton) //right button -> translation
    {
        camera->cameraTranslate(d);
    }
    else if(event->buttons() & Qt::MidButton)
    {
        camera->modelRot(d);
    }

    _mouse_pos = p;
}

void GLViewer::wheelEvent(QWheelEvent *event)
{
    auto p = event->pos();

//    qDebug() << event->delta(); //每次delta都是+/- 120，每滚动一格都会进入，+1即可
    if(event->delta() < 0)
    {
//        camera->zoom = camera->zoom * 1.13 > 150 ? 150 : camera->zoom * 1.13;
        camera->cameraZoom(0.2f);
    }
    else if(event->delta() > 0)
    {
//        camera->zoom = camera->zoom * 0.885 < 0.1 ? 0.1 : camera->zoom * 0.885;
        camera->cameraZoom(-0.2f);
    }

    update();
}

void GLViewer::setStatus(QStringList &lst)
{
    status = lst;
}

void GLViewer::addStatus(QString &s)
{
    status << s;
}

void GLViewer::clearStatus()
{
    status.clear();
}

void GLViewer::drawStatus()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //需要恢复成GL_FILL模式，否则显示的文字看不清
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    for(int i = 0; i < status.size(); i++)
    {
        painter.drawText(5, 15+20*i, status[i]);
    }
}

void GLViewer::dataProc(const QVector<QVector3D> &data)
{
    nodes = data;
    for(auto& node : nodes)
    {
        node *= 0.001;
    }
}

void GLViewer::toolProc(const QVector<NdiTool> &data)
{
    existTools = data;
}

void GLViewer::toolsLoaded(const QVector<NdiTool> &tools)
{

    for(auto& tool : tools)
    {

        toolModels[tool.name] = new GLModel(QFileInfo(tool.modelPath).absoluteFilePath().toStdString());
        toolModels[tool.name]->setColor(QVector4D(131.0f/255.0,111.0f/255.0, 255.0f/255.0, 1.0f));

    }
}
//加载tooldef.xml文件
void GLViewer::loadTools()
{
    //新的HoloLens2
//    tools = XmlParser::getToolsByDistancesFromXml("./tooldef-hololen2-2.xml");
    //旧的HoloLens2
    //tools = XmlParser::getToolsByDistancesFromXml("./tooldef-hololen2.xml");
    //HoloLens1
    //    QFile file("../MotionViewer/tooldef.xml");//当前执行路径文件在：C:\Users\Ausland\Documents\GitHub\MotionViewer\build-MotionViewer-Desktop_Qt_5_12_8_MSVC2017_64bit-Debug
        //新的HoloLens2
    //    QFile file("../MotionViewer/tooldef-hololen2-2.xml");
        //目前新粘贴过的holoLens2
       // QFile file("../MotionViewer/tooldef-hololen2.xml");
        //用泡沫胶粘贴的HoloLens1
        //QFile file("../MotionViewer/1.xml");


    tools = XmlParser::getToolsByDistancesFromXml("../MotionViewer/1.xml");

    toolsLoaded(tools);
}
