#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
//#include <QElapsedTimer> //计算时间流逝
#include <memory>
#include "backdrop.h"

namespace Ui {
class GLViewer;
}

class GLViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLViewer(QWidget *parent = nullptr);
    ~GLViewer();

private:
    Ui::GLViewer *ui;
    Backdrop *backdrop; //draw background

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // GLVIEWER_H
