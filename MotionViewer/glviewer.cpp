#include "glviewer.h"
#include "ui_glviewer.h"
#include <QTimer>


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
    f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    etimer = std::make_shared<QElapsedTimer>();
    etimer->start();
}

void GLViewer::paintGL()
{
    float r = sin(etimer->elapsed()*0.001);
    float g = sin(etimer->elapsed()*0.001 + 1.57);
    float b = sin(etimer->elapsed()*0.001 + 0.717);
    f->glClearColor(r,g,b,1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT);
}

void GLViewer::resizeGL(int w, int h)
{

}
