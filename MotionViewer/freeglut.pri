win32:{
!build_pass:message(FreeGLUT Directory is $$(FREEGLUT))
INCLUDEPATH += $$(FREEGLUT)/include
    Debug:{
        LIBS += $$(FREEGLUT)/lib/freeglutd.lib
#        LIBS += $$(FREEGLUT)/lib/freeglut_staticd.lib
    }
    Release:{
        LIBS += $$(FREEGLUT)/lib/freeglut.lib
#        LIBS += -l$$(FREEGLUT)/lib/freeglut_static
    }
}

unix:!macx {
	CONFIG += link_pkgconfig
        PKGCONFIG += freeglut
}

unix:macx {
	INCLUDEPATH += "/usr/local/include"
        LIBS += -L"/usr/local/lib" -lfreeglut
}

