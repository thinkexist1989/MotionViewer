#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ndiviewer.h"
#include "ndicomm.h"
#include "holocomm.h"
#include "holoviewer.h"

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

private:
    Ui::MainWindow *ui;

    NdiViewer  *ndiViewer;
    NdiComm    *ndiComm;

    HoloComm   *holoComm;
    HoloViewer *holoViewer;


    void setStatusMsg(QString msg);
};

#endif // MAINWINDOW_H
