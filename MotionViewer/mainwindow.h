#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QSettings>

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

    void on_actionNdiViewer_toggled(bool arg1);

    void on_actionHoloViewer_toggled(bool arg1);

    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

private:
    Ui::MainWindow *ui;

    NdiComm    *ndiComm;
    NdiViewer  *ndiViewer;

    HoloComm   *holoComm;
    HoloViewer *holoViewer;

    QTranslator *chineseTranslator;

    void setStatusMsg(QString msg);

    void loadSettings();
    void saveSettings();

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
