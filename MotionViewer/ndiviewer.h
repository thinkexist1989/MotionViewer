#ifndef NDIVIEWER_H
#define NDIVIEWER_H

#include <QWidget>
#include <QList>
#include <QVector3D>
#include <QMatrix4x4>

namespace Ui {
class NdiViewer;
}

class NdiViewer : public QWidget
{
    Q_OBJECT

public:
    explicit NdiViewer(QWidget *parent = nullptr);
    ~NdiViewer();

private:
    Ui::NdiViewer *ui;

    void refreshMarkersView(QList<QVector3D> data);
    void refreshMatrixView(QMatrix4x4 mat);
    void init();

public slots:
    void dataProc(QList<QVector3D> data); //Process markers' coordinates

protected:
    void changeEvent(QEvent *event);
};

#endif // NDIVIEWER_H
