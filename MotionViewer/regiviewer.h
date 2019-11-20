#ifndef REGIVIEWER_H
#define REGIVIEWER_H

#include <QWidget>
#include <QMatrix4x4>

namespace Ui {
class RegiViewer;
}

class RegiViewer : public QWidget
{
    Q_OBJECT

public:
    explicit RegiViewer(QWidget *parent = nullptr);
    ~RegiViewer();

    void showMatrix(QMatrix4x4 mat);

    QMatrix4x4 poindCloudRegiMat;

private slots:
    void on_btnOpenFile_clicked();

    void on_btnLoad_clicked();

private:
    Ui::RegiViewer *ui;
};

#endif // REGIVIEWER_H
