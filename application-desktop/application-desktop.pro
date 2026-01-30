QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    applicationinfodialog.cpp \
    applicationmanagerwidget.cpp \
    availableapplicationswidget.cpp \
    availableitemwidget.cpp \
    filterproxymodel.cpp \
    main.cpp \
    mainwindow.cpp \
    searchwidget.cpp

HEADERS += \
    applicationinfodialog.h \
    applicationmanagerwidget.h \
    availableapplicationswidget.h \
    availableitemwidget.h \
    filterproxymodel.h \
    mainwindow.h \
    searchwidget.h

FORMS += \
    applicationinfodialog.ui \
    applicationmanagerwidget.ui \
    availableapplicationswidget.ui \
    availableitemwidget.ui \
    mainwindow.ui \
    searchwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../application-core/release/ -lapplication-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../application-core/debug/ -lapplication-core
else:unix: LIBS += -L$$OUT_PWD/../application-core/ -lapplication-core

INCLUDEPATH += $$PWD/../application-core
DEPENDPATH += $$PWD/../application-core

RESOURCES += \
    Resource.qrc
