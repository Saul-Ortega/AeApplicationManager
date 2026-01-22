QT -= gui

TEMPLATE = lib
DEFINES += APPLICATION_CORE_LIBRARY

CONFIG += c++17

SOURCES += \
    Version.cpp \
    application.cpp \
    applicationcore.cpp \
    applicationdao.cpp

HEADERS += \
    Version.h \
    application-core_global.h \
    application.h \
    applicationcore.h \
    applicationdao.h

unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resource.qrc
