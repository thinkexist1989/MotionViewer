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
    QMap<QString,QList<QVector3D>> ToolsNumAndPose;
private:
    Ui::NdiViewer *ui;

    void refreshMarkersView(QList<QVector3D> data);
    //void judgeTool(QList<QVector3D> data);
    void refreshMatrixView(QMatrix4x4 mat);
    void init();
    bool NdiViewer::isTool(double *distance,QList<float>);
    QMap<QString,int> judgeTool(double *dis,  QString &toolname);
    QMatrix4x4 getVirtualTransformMatrix(); // Virtual tool pose
    QMatrix4x4 getRealTransformMatrix(); // Real tool pose
    QMatrix4x4 getCalibrationMatrix(); //Calibration Matrix
    QMap<QString,QList<QVector3D>> getToolsNumAndPose(QList<QVector3D> data);//tool number and pose
public slots:
    void dataProc(QList<QVector3D> data); //Process markers' coordinates

protected:
    void changeEvent(QEvent *event);
private slots:
    void on_btnExec_clicked();
    void on_cmbSteps_currentIndexChanged(int index);
};

#endif // NDIVIEWER_H
