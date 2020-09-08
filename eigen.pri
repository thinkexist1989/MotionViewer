win32:{
!build_pass:message(Eigen Directory is $$(EIGEN))
INCLUDEPATH += $$(EIGEN)
    Debug:{
        LIBS += -l$$(OPENCV)/lib/opencv_world345d
    }
    Release:{
        LIBS += -l$$(OPENCV)/lib/opencv_world345
    }
}

unix:!macx {
        CONFIG += link_pkgconfig
        PKGCONFIG += eigen3
}

unix:macx {
        INCLUDEPATH += "/usr/local/include"
        LIBS += -L"/usr/local/lib" -lopencv_world
}

