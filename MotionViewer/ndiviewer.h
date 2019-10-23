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

    QMatrix4x4 virtualTransformMatrix;
    QMatrix4x4 realTransformMatrix;
    QMatrix4x4 calibrationMatrix;

private:
    Ui::NdiViewer *ui;

    void refreshMarkersView(QList<QVector3D> data);
    void refreshMatrixView(QMatrix4x4 mat);
    void init();

    QMatrix4x4 getVirtualTransformMatrix(); // Virtual tool pose
    QMatrix4x4 getRealTransformMatrix(); // Real tool pose
    QMatrix4x4 getCalibrationMatrix(); //Calibration Matrix

public slots:
    void dataProc(QList<QVector3D> data); //Process markers' coordinates

protected:
    void changeEvent(QEvent *event);
private slots:
    void on_btnExec_clicked();
    void on_cmbSteps_currentIndexChanged(int index);
};

#endif // NDIVIEWER_H
