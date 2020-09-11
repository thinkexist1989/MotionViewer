#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QSettings>
#include <QThread>

#include "ndiviewer.h"
#include "ndicomm.h"
#include "holocomm.h"
#include "holoviewer.h"
#include "regiviewer.h"
#include "transform.h"
#include "nditool.h"
#include "glviewer.h"


#include <QMetaType> //register the signal type

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionNdiViewer_toggled(bool arg1);

    void on_actionHoloViewer_toggled(bool arg1);

    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

    void on_ndiCommDockWidget_visibilityChanged(bool visible);

    void on_holoCommDockWidget_visibilityChanged(bool visible);

    void on_actionRegi_toggled(bool arg1);

    void on_regiDockWidget_visibilityChanged(bool visible);

    void on_actionDisplayMode_triggered();

    void on_actionNdi_triggered();

    void on_actionFrontView_triggered();

    void on_actionRightView_triggered();

    void on_actionTopView_triggered();

    void on_actionAxo_triggered();

private:
    Ui::MainWindow *ui;

    NdiComm    *ndiComm;
    NdiViewer  *ndiViewer;

    HoloComm   *holoComm;
    HoloViewer *holoViewer;

    RegiViewer *regiViewer;

    GLViewer *glViewer;

    Transform  *transform;

    QThread *transformThread;

    QTranslator *chineseTranslator;

    void setStatusMsg(QString msg);

    void loadSettings();
    void saveSettings();

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
