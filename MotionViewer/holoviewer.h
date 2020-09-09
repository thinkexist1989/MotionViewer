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

public slots:
    void dataProc(const QString &);

protected:
    void changeEvent(QEvent *event);

};

#endif // HOLOVIEWER_H
