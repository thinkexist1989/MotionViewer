#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
//#include <QElapsedTimer> //计算时间流逝
#include <memory>
#include "glbackdrop.h"
#include "glmodel.h"
#include "glcoordinate.h" //GLCo
#include "nditool.h" //NdiTool类
#include "glcamera.hpp" //GLCamera类

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

    GLModel* nodeModel;
    GLModel* ndiModel;
    GLModel* tipModel;

    QVector<NdiTool> tools; //预加载的工具
    QMap<QString, GLModel*> toolModels; //预加载的所有工具3D model，用名称对应

    QVector<QVector3D> nodes; //ndi detected nodes
    QVector<NdiTool> existTools; //检测到的工具

    GLCamera* camera; //GLCamera类，负责观察矩阵计算


    void setDrawMode(int mode);
    void setProjectionMode(int mode);

private:

//    void drawAxis();

    void drawNodes(); //draw NDI node

private:
    Ui::GLViewer *ui;

    GLBackdrop *backdrop; //draw background

    GLCoordinate* coordinate;

    QVector3D center;
    float scale;

    float perspective;
    int drawMode = 0; // 0 is shaded, 1 is wireframe
    int projMode = 0; //projection.  0 is perspective , 1 is orthogonal

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

    QMatrix4x4 model; //

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
    void toolProc(const QVector<NdiTool>& data);
    void toolsLoaded(const QVector<NdiTool>& existTools);

public:
    void loadTools();

};

#endif // GLVIEWER_H
