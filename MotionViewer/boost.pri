win32:{
!build_pass:message(Boost Directory is $$(BOOST))
INCLUDEPATH += $$(BOOST)/include/boost-1_72/
    Debug:{
        LIBS += $$(BOOST)/lib/libboost*.lib
    }
    Release:{
        LIBS += $$(BOOST)/lib/libboost*.lib
    }
}

unix:!macx {
	CONFIG += link_pkgconfig
        PKGCONFIG += boost
}

unix:macx {
	INCLUDEPATH += "/usr/local/include"
        LIBS += -L"/usr/local/lib" -lboost
}

