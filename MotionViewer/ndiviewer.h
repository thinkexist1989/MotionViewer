#ifndef NDIVIEWER_H
#define NDIVIEWER_H

#include <QWidget>
#include <QList>
#include <QVector3D>
#include <QMatrix4x4>
#include <nditool.h>
#include <transform.h>
#include <QPair>

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

    void refreshToolView(QList<NdiTool> tools);// show detected tools
    void refreshMarkersInTool(NdiTool tool);


    void init();
    bool isTool(QVector<float> dists, NdiTool toolx, int &index);
    QList<NdiTool> getToolDefination();
    QPair<QString, int> judgeTool(QVector<float> dists);

    QMatrix4x4 getVirtualTransformMatrix(); // Virtual tool pose
    QMatrix4x4 getRealTransformMatrix(); // Real tool pose
    QMatrix4x4 getCalibrationMatrix(); //Calibration Matrix

    QList<NdiTool> getTools(QList<QVector3D> data);//tool name and pose
    QList<NdiTool> tools;
    void getRegiMat();

signals:
    void readyForTransform(int,QList<NdiTool>); //signal for transform
    void needRegiMat(QString fileName);
    void toolsReady(QList<NdiTool>);

public slots:
    void dataProc(QList<QVector3D> data); //Process markers' coordinates

protected:
    void changeEvent(QEvent *event);
private slots:
    void on_btnExec_clicked();
    void on_cmbSteps_currentIndexChanged(int index);

    void on_lstToolName_currentTextChanged(const QString &currentText);
};

#endif // NDIVIEWER_H
