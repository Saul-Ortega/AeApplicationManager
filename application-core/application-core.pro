QT += core gui widgets

TEMPLATE = lib
DEFINES += APPLICATION_CORE_LIBRARY

CONFIG += c++17

SOURCES += \
    application.cpp \
    applicationcore.cpp \
    applicationdao.cpp \
    applicationmodel.cpp \
    installerworker.cpp \
    version.cpp \
    versionmodel.cpp

HEADERS += \
    application-core_global.h \
    application.h \
    applicationcore.h \
    applicationdao.h \
    applicationmodel.h \
    installerworker.h \
    version.h \
    version.h \
    versionmodel.h

unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES +=
