#include "glviewer.h"
#include "ui_glviewer.h"
#include <QTimer>
#include <QPainter>

#include <QMouseEvent> //process mouse event
#include <QWheelEvent> //process wheel event
#include <QFileInfo>
#include <QtMath>


GLViewer::GLViewer(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::GLViewer),
    yaw(-90), //当yaw= 0时，在xz平面的分量指向x轴，而初始值应该指向-z，所以旋转-90度
    pitch(0),
    fov(45.0),
    cameraPos(1.0, 1.0, 1.0),
    cameraFront(-0.2, -0.2, -0.2),
    cameraUp(0.0, 1.0, 0.0)
{
    ui->setupUi(this);
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

void GLViewer::drawAxis()
{
    initializeOpenGLFunctions();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int h = height();
    int w = width();

    if (w > h)
        glOrtho(-w / h, w / h, -1.0f, 1.0f, -1.0f, 1.0f);
    else
        glOrtho(-1.0f, 1.0f, -h / w, h / w, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
//    glRotatef(y_angle, 0.0f, 1.0f, 0.0f);

    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f); //画红色的x轴
    glBegin(GL_LINES);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
    glColor3f(0.0, 1.0, 0.0); //画绿色的y轴
    glBegin(GL_LINES);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glColor3f(0.0, 0.0, 1.0); //画蓝色的z轴
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
}

void GLViewer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //default background

    backdrop = new Backdrop();

    coordinate = new Coordinate();

//    model1 = new Model("C:\\Users\\think\\Desktop\\universal_robot-melodic-devel\\ur_e_description\\meshes\\ur5e\\visual\\base.dae");

    model1 = new Model(QFileInfo("./sphere.obj").absoluteFilePath().toStdString());

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{update();});
    timer->start(10);
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

    glClearColor(0.2f, 0.3f, 0.3f, 0.0f); //default background

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); //enable depth test


    /*** view and projection matrix ***/
    QMatrix4x4 view;
    view.lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

    QMatrix4x4 projection;
    projection.perspective(fov, (float)width()/height(), 0.1, 100);

    /*** background ***/
//    backdrop->draw();

    /*** axis ***/
//    drawAxis();
    QMatrix4x4 model;
    coordinate->draw(view, projection, model);

    /*** model ***/
    QMatrix4x4 trans;
    //    model.rotate(20.0f, rotAxis[i]);
    trans.scale(0.001f, 0.001f, 0.001f);
    trans.translate(0.0f, 0.0f, 0.0f);
    model1->draw(view, projection, trans);

    QMatrix4x4 trans2;
    trans2.scale(0.001f, 0.001f, 0.001f);
    trans2.translate(5.0f, 5.0f, 5.0f);
    model1->draw(view, projection, trans2);


    /*** status ***/
    QString s1 = QString("Status: Dectected %1 nodes.").arg(rand()%100);
    QString s2 = QString("        Phased %1 tools.").arg(rand()%10);
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
        setCursor(Qt::ClosedHandCursor); //set cursor style
    }
    else if(event->button() == Qt::RightButton)
    {
        _mouse_pos = event->pos();
        setCursor(Qt::SizeAllCursor);
    }
}

void GLViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton ||
        event->button() == Qt::RightButton)
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
        yaw = fmod(yaw + d.x()*0.1, 360); //fmod is like % operator for float
        tilt = fmod(tilt - d.y()*0.1, 360);
        pitch = fmod(pitch - d.y()*0.1, 360);

        QVector3D front(cos(qDegreesToRadians(pitch))*cos(qDegreesToRadians(yaw)),
                        sin(qDegreesToRadians(pitch)),
                        cos(qDegreesToRadians(pitch))*sin(qDegreesToRadians((yaw))));
        cameraFront = front.normalized();

//        qDebug() << QString("yaw: %1, tilt: %2").arg(yaw).arg(tilt).toStdString().c_str();
        update();
    }
    else if(event->buttons() & Qt::RightButton) //right button -> translation
    {
        cameraPos += d.x()/500.0*QVector3D::crossProduct(cameraFront, cameraUp).normalized();
        cameraPos -= d.y()/500.0*cameraUp;
    }

    _mouse_pos = p;
}

void GLViewer::wheelEvent(QWheelEvent *event)
{
    auto p = event->pos();

//    qDebug() << event->delta(); //每次delta都是+/- 120，每滚动一格都会进入，+1即可
    if(event->delta() < 0)
    {
        zoom *= 1.13;
        fov *= 1.13;
        fov = fov < 0.05? 0.05: fov;
    }
    else if(event->delta() > 0)
    {
        zoom /= 1.13;
        fov /= 1.13;
        fov = fov > 70? 70 : fov;
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
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    for(int i = 0; i < status.size(); i++)
    {
        painter.drawText(5, 15+20*i, status[i]);
    }
}
