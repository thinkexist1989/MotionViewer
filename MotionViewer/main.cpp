#include "mainwindow.h"
#include <QApplication>
#include <GL/freeglut.h>

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

//    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.showMaximized(); Full Screen Show

    return a.exec();
}
