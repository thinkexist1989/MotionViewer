#ifndef NDIVIEWER_H
#define NDIVIEWER_H

#include <QWidget>

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
};

#endif // NDIVIEWER_H
