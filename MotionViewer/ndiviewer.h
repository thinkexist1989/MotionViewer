#ifndef NDIVIEWER_H
#define NDIVIEWER_H

#include <QWidget>
#include <QList>
#include <QVector3D>
#include <QMatrix4x4>
#include <nditool.h>

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
    QList<NdiTool> ToolStates;
    QList<NdiTool> toolInfos;
private:
    Ui::NdiViewer *ui;

    void refreshMarkersView(QList<QVector3D> data); //show marker detected by NDI
    void refreshMatrixView(QMatrix4x4 mat);

    void refreshToolView(QMap<QString,QList<QVector3D>> toolStatus);// show detected tools
    void refreshMarkersInTool(NdiTool tool);


    void init();
    bool isTool(double *distance,QList<float> toolSize,int &count);
    QMap<QString,QList<float>>getToolDefination();
    QMap<QString,int> judgeTool(double *dis,  QString &toolname,int &count);

    QMatrix4x4 getVirtualTransformMatrix(); // Virtual tool pose
    QMatrix4x4 getRealTransformMatrix(); // Real tool pose
    QMatrix4x4 getCalibrationMatrix(); //Calibration Matrix

    QMap<QString,QList<QVector3D>> getToolsNumAndPose(QList<QVector3D> data);//tool name and pose
public slots:
    void dataProc(QList<QVector3D> data); //Process markers' coordinates

protected:
    void changeEvent(QEvent *event);
private slots:
    void on_btnExec_clicked();
    void on_cmbSteps_currentIndexChanged(int index);
};

#endif // NDIVIEWER_H
