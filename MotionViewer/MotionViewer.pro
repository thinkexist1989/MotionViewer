#-------------------------------------------------
#
# Project created by QtCreator 2019-10-12T15:43:25
#
#-------------------------------------------------

QT       += core gui network serialport xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    glbackdrop.cpp \
    glcoordinate.cpp \
    glmesh.cpp \
    glmodel.cpp \
    glviewer.cpp \
        main.cpp \
        mainwindow.cpp \
    aboutdlg.cpp \
    ndicomm.cpp \
    ndiviewer.cpp \
    holocomm.cpp \
    holoviewer.cpp \
    nditool.cpp \
    stb_image.cpp \
    transform.cpp \
    regiviewer.cpp \
    xmlparser.cpp

HEADERS += \
    glbackdrop.h \
    glcamera.hpp \
    glcoordinate.h \
    glmesh.h \
    glmodel.h \
    glviewer.h \
        mainwindow.h \
    aboutdlg.h \
    ndicomm.h \
    ndiviewer.h \
    holocomm.h \
    holoviewer.h \
    nditool.h \
    stb_image.h \
    transform.h \
    regiviewer.h \
    xmlparser.h

FORMS += \
    glviewer.ui \
        mainwindow.ui \
    aboutdlg.ui \
    ndiviewer.ui \
    ndicomm.ui \
    holocomm.ui \
    holoviewer.ui \
    regiviewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gl/gl.qrc \
    resources.qrc

# 程序logo
RC_ICONS = viewer.ico

# Add OpenCV config
#include(opencv.pri)

# Add Eigen config
#include(eigen.pri)

# Add Assimp config
include(assimp.pri)

# Add freeglut config
#include(freeglut.pri)

win32:LIBS += -lOpenGL32
win32:LIBS += -lGlu32


# Add translations
TRANSLATIONS += translation_zh.ts

#message($${PWD})


linux-g++{
    #...
    EXTRA_BINFILES += \
        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstrtp.so \
        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstvideo4linux2.so
    for(FILE,EXTRA_BINFILES){
        QMAKE_POST_LINK += $$quote(cp $${FILE} $${DESTDIR}$$escape_expand(\n\t))
    }
}

win32 {
    #...
    EXTRA_BINFILES += \
        $$PWD/gl/sphere.dae \
        $$PWD/gl/ndi.stl \
        $$PWD/gl/hololens.stl \
        $$PWD/gl/needle.stl \
        $$PWD/gl/bonedrill.stl \
        $$PWD/1.xml
    EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
    EXTRA_BINFILES_WIN ~= s,/,\\,g
        DESTDIR_WIN = $${OUT_PWD}
    DESTDIR_WIN ~= s,/,\\,g
    for(FILE,EXTRA_BINFILES_WIN){
                QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
    }
}
