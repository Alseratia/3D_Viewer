TARGET = 3DViewer
CONFIG += c++17 warn_on
QT     += core gui widgets
unix: {
    QT       += opengl
    LIBS     += -lOpenGL
}

win32: {
    QT      += openglwidgets
    LIBS    += -lopengl32
}

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
include(../libs/lib_qt_gifimage/src/gifimage/qtgifimage.pri)

HEADERS += \
            view/configurator.h \
            view/customwidgets.h \
            view/drawwindow.h \
            view/mainwindow.h \
            model/model.h \
            controller/controller.h \
            view/settingmanager.h

SOURCES += \
            main.cpp \
            view/configurator.cpp \
            view/drawwindow.cpp \
            view/mainwindow.cpp \
            model/model.cpp \
  view/settingmanager.cpp

FORMS += view/mainwindow.ui
RESOURCES = styles/breeze.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
