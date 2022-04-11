QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    datedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    newtaskdialog.cpp \
    preferences.cpp \
    task.cpp \
    taskfiltermodel.cpp \
    taskmodel.cpp \
    updatetodaytimeworker.cpp

HEADERS += \
    aboutdialog.h \
    datedialog.h \
    mainwindow.h \
    newtaskdialog.h \
    preferences.h \
    task.h \
    taskfiltermodel.h \
    taskmodel.h \
    updatetodaytimeworker.h

FORMS += \
    aboutdialog.ui \
    datedialog.ui \
    mainwindow.ui \
    newtaskdialog.ui \
    preferences.ui

TRANSLATIONS += \
    UnderManager_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

