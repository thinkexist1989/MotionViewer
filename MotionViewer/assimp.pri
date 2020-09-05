win32:{
!build_pass:message(Assimp Directory is $$(ASSIMP))
INCLUDEPATH += $$(ASSIMP)/include
    Debug:{
        LIBS += $$(ASSIMP)/lib/assimp-vc140-mt.lib
    }
    Release:{
        LIBS += $$(ASSIMP)/lib/assimp-vc140-mt.lib
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

