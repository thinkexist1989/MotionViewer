#ifndef REGIVIEWER_H
#define REGIVIEWER_H

#include <QWidget>

namespace Ui {
class RegiViewer;
}

class RegiViewer : public QWidget
{
    Q_OBJECT

public:
    explicit RegiViewer(QWidget *parent = nullptr);
    ~RegiViewer();

private:
    Ui::RegiViewer *ui;
};

#endif // REGIVIEWER_H
