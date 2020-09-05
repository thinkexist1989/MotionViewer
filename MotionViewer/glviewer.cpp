#include "glviewer.h"
#include "ui_glviewer.h"
#include <QTimer>
#include <QPainter>


GLViewer::GLViewer(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::GLViewer)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{update();});
    timer->start(10);
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

    backdrop = new Backdrop();

}

void GLViewer::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); //开启深度测试

    backdrop->draw();


    QString s1 = QString("Status: Dectected %1 nodes.").arg(rand()%100);
    QString s2 = QString("        Phased %1 tools.").arg(rand()%10);


    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawText(5, 15, s1);
    painter.drawText(5, 35, s2);
//    painter.drawText(10, height() - 10, "status");

}

void GLViewer::resizeGL(int w, int h)
{

}
