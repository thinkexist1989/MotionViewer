#ifndef HOLOVIEWER_H
#define HOLOVIEWER_H

#include <QWidget>

namespace Ui {
class HoloViewer;
}

class HoloViewer : public QWidget
{
    Q_OBJECT

public:
    explicit HoloViewer(QWidget *parent = nullptr);
    ~HoloViewer();

private:
    Ui::HoloViewer *ui;

};

#endif // HOLOVIEWER_H
