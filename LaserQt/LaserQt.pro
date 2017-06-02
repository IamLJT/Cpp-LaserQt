#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T13:07:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network datavisualization xml concurrent

TARGET = LaserQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    stack_window.cpp \
    main_window.cpp \
    second_window.cpp \
    third_window.cpp \
    fourth_window.cpp \
    auxiliary_function.cpp \
    qcustomplot.cpp \
    custom_qlabel.cpp \
    image_viewer.cpp


HEADERS  += \
    stack_window.h \
    main_window.h \
    second_window.h \
    third_window.h \
    fourth_window.h \
    auxiliary_function.h \
    my_messagebox.h \
    qcustomplot.h \
    custom_qlabel.h \
    image_viewer.h


FORMS    +=

RESOURCES += \
    laserqt.qrc

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

# QJson setting
win32 {
    INCLUDEPATH += include_cpp/qjson
    LIBS += -L$$PWD/winlib/qjson -lqjson-qt5
} else {
    INCLUDEPATH += include_cpp/qjson
    LIBS += -L$$PWD/lib/qjson -lqjson-qt5
}

# QCustomPlot setting
win32 {
    LIBS += -L$$PWD/winlib/qcustomplot -lqcustomplotd2
} else {
    LIBS += -L$$PWD/lib/qcustomplot -lqcustomplot -lqcustomplotd
}

# libxl setting
win32 {
    QT += xlsx
} else {
    INCLUDEPATH += include_cpp/libxl
    LIBS += -L$$PWD/lib/libxl/lib64 -lxl

    QMAKE_LFLAGS_DEBUG = "-Wl,-rpath,$$PWD/lib/libxl/lib64/"
    QMAKE_LFLAGS_RELEASE = "-Wl,-rpath,$$PWD/lib/libxl/lib64/"
}

# algorithm setting
win32 {
    INCLUDEPATH += include_cpp/algorithm
} else {

}

# Boost setting
win32 {
    INCLUDEPATH += C:/Boost/include
    INCLUDEPATH += C:/Boost/include/boost
} else {
    INCLUDEPATH += /usr/local/include/boost
}

# KDTree setting
win32 {
    INCLUDEPATH += include_cpp/kdtree
    LIBS += -L$$PWD/winlib/kdtree -lkdtree
} else {
    INCLUDEPATH += include_cpp/kdtree
    LIBS += -L$$PWD/lib/kdtree -lkdtree
}
