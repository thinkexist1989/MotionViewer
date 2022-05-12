/**
 * @file main.cpp
 * @author Yang Luo (luoyang@sia.cn)
 * @brief this is the program entry of MotionViewer
 * @version 0.1
 * @date 2020-09-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

/**
 * @brief Main function
 * 
 * @param argc numbers of arguments
 * @param argv array of arguments
 * @return int 
 * 项目入口
 */
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  //!< Supported high DPI
//    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.showMaximized(); Full Screen Show

    return a.exec();
}
