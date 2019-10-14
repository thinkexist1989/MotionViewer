#ifndef HOLOCOMM_H
#define HOLOCOMM_H

#include <QWidget>

namespace Ui {
class HoloComm;
}

class HoloComm : public QWidget
{
    Q_OBJECT

public:
    explicit HoloComm(QWidget *parent = nullptr);
    ~HoloComm();

private:
    Ui::HoloComm *ui;
};

#endif // HOLOCOMM_H
