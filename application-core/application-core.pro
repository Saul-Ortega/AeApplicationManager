QT -= gui

TEMPLATE = lib
DEFINES += APPLICATION_CORE_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    applicationcore.cpp \
    applicationdao.cpp \
    applicationmodel.cpp \
    version.cpp

HEADERS += \
    application-core_global.h \
    application.h \
    applicationcore.h \
    applicationdao.h \
    applicationmodel.h \
    version.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    applications.json
