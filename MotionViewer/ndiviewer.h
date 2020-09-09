#ifndef NDIVIEWER_H
#define NDIVIEWER_H

#include <QWidget>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
#include <nditool.h>
#include <transform.h>
#include <QPair>
#include <xmlparser.h>

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
    QMap<QString,QVector<QVector3D>> ToolsNumAndPose;
    QVector<NdiTool> ToolStates;
    QVector<NdiTool> toolInfos;
private:
    Ui::NdiViewer *ui;

    void refreshMarkersView(const QVector<QVector3D>& data); //show marker detected by NDI
    void refreshMatrixView(QMatrix4x4 mat);

    void refreshToolView(QVector<NdiTool> tools);// show detected tools
    void refreshMarkersInTool(NdiTool tool);


    void init();
    bool isTool(QVector<float> dists, NdiTool toolx, int &index);
    //QVector<NdiTool> getToolDefination();
    void getToolDefination();
    QPair<QString, int> judgeTool(QVector<float> dists);

    QMatrix4x4 getVirtualTransformMatrix(); // Virtual tool pose
    QMatrix4x4 getRealTransformMatrix(); // Real tool pose
    QMatrix4x4 getCalibrationMatrix(); //Calibration Matrix

    QVector<NdiTool> getTools(const QVector<QVector3D>& data);//tool name and pose
    QVector<NdiTool> tools;
    QVector<NdiTool> existTools;
    void getRegiMat();

signals:
    void readyForTransform(int,QVector<NdiTool>); //signal for transform
    void needRegiMat(QString fileName);
    void toolsReady(QVector<NdiTool>);

public slots:
    void dataProc(const QVector<QVector3D>& data); //Process markers' coordinates

protected:
    void changeEvent(QEvent *event);
private slots:
    void on_btnExec_clicked();
    void on_cmbSteps_currentIndexChanged(int index);

    void on_lstToolName_currentTextChanged(const QString &currentText);
};

#endif // NDIVIEWER_H
