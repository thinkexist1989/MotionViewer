#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QElapsedTimer>
#include <memory>


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
//    QOpenGLFunctions *f;
    std::shared_ptr<QElapsedTimer> etimer;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // GLVIEWER_H
