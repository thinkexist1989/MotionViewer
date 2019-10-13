!build_pass:message(OpenCV Directory is $$(OPENCV))
INCLUDEPATH += $$(OPENCV)/include

win32:{
    Debug:{
        LIBS += -l$$(OPENCV)/lib/opencv_world345d
    }
    Release:{
        LIBS += -l$$(OPENCV)/lib/opencv_world345
    }
}

unix:!macx {
	CONFIG += link_pkgconfig
	PKGCONFIG += opencv
}

unix:macx {
	INCLUDEPATH += "/usr/local/include"
	LIBS += -L"/usr/local/lib" -lopencv_world
}

