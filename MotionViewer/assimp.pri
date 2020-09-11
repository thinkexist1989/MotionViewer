win32:{
#!build_pass:message(Assimp Directory is $$(ASSIMP))
INCLUDEPATH += $$(ASSIMP)/include
    Debug:{
#        LIBS += $$(ASSIMP)/lib/assimp-vc142-mt.lib
        LIBS += $$(ASSIMP)/lib/assimp-*.lib
    }
    Release:{
#        LIBS += $$(ASSIMP)/lib/assimp-vc142-mt.lib
        LIBS += $$(ASSIMP)/lib/assimp-*.lib
    }
}

unix:!macx {
	CONFIG += link_pkgconfig
        PKGCONFIG += assimp
}

unix:macx {
	INCLUDEPATH += "/usr/local/include"
        LIBS += -L"/usr/local/lib" -lassimp
}

