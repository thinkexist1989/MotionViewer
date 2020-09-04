#include "glviewer.h"
#include "ui_glviewer.h"
#include <QTimer>
#include <GL/freeglut.h>


GLViewer::GLViewer(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::GLViewer)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{update();});
    timer->start(50);
}

GLViewer::~GLViewer()
{
    delete ui;
}

void GLViewer::initializeGL()
{
//    f = QOpenGLContext::currentContext()->functions();
//    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    initializeOpenGLFunctions();

//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

//    glEnable(GL_DEPTH_TEST); //开启深度测试

    etimer = std::make_shared<QElapsedTimer>();
    etimer->start();

}

void GLViewer::paintGL()
{
//    float r = sin(etimer->elapsed()*0.001);
//    float g = sin(etimer->elapsed()*0.001 + 1.57);
//    float b = sin(etimer->elapsed()*0.001 + 0.717);
//    f->glClearColor(r,g,b,1.0f);
//    f->glClear(GL_COLOR_BUFFER_BIT);
//    glClearColor(r,g,b,1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);


//    glutSolidSphere(3.0f,100,100);
    glMatrixMode(GL_MODELVIEW);
    // clear the drawing buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // clear the identity matrix.
    glLoadIdentity();


    glTranslatef(0.0, 0.0, -4.0);
    // Red color used to draw.
    glColor3f(0.8, 0.2, 0.1);
    // changing in transformation matrix.
    // rotation about X axis
    glRotatef(0, 50.0, 0.0, 0.0);
    // rotation about Y axis
    glRotatef(0, 0.0, 0.0, 0.0);
    // rotation about Z axis
    glRotatef(0, 0.0, 0.0, 0.0);
    // scaling transfomation
    glScalef(1.0, 1.0, 1.0);
    // built-in (glut library) function , draw you a sphere.
//    glutSolidSphere(2, 20, 50);
    // Flush buffers to screen

    glFlush();
}

void GLViewer::resizeGL(int w, int h)
{

}
