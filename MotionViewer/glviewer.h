#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
//#include <QElapsedTimer> //计算时间流逝
#include <memory>
#include "backdrop.h"
#include "model.h"
#include "coordinate.h"

namespace Ui {
class GLViewer;
}

class GLViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLViewer(QWidget *parent = nullptr);
    ~GLViewer();

    void drawShaded();
    void drawWireframe();

    Model* nodeModel;

    QVector<QVector3D> nodes; //ndi detected nodes

private:
    void setDrawMode(int mode);
//    void drawAxis();

    void drawNodes(); //draw NDI node

private:
    Ui::GLViewer *ui;

    Backdrop *backdrop; //draw background

    Coordinate* coordinate;

    QVector3D center;
    float scale;
    float zoom;
    float tilt;  // up down
    float yaw;  //left right
    float pitch;

    float fov; //field of view

    float perspective;
    int drawMode; // 0 is shaded, 1 is wireframe

//    Q_PROPERTY(float perspective MEMBER perspective WRITE set_perspective);
//    QPropertyAnimation anim;

    QPoint _mouse_pos; //previous mouse position

    //camera related
    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;

    QStringList status; //print status on GL viewport

    QMatrix4x4 view; //view matrix
    QMatrix4x4 projection; //projection matrix

protected:
    void initializeGL() override; //intialize OpenGL
    void paintGL() override; //draw OpenGL
    void resizeGL(int w, int h) override; //resize OpenGL

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

public slots:
    void setStatus(QStringList& lst);
    void addStatus(QString& s);
    void clearStatus();
    void drawStatus();

    void dataProc(const QVector<QVector3D>& data);

};

#endif // GLVIEWER_H
