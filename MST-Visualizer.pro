QT       += core gui widgets

CONFIG += c++17

TARGET = MST-Visualizer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    graphview.cpp \
    mstalgorithms.cpp \
    graphgenerator.cpp \
    benchmarkdialog.cpp \
    edgedialog.cpp

HEADERS += \
    mainwindow.h \
    graphview.h \
    mstalgorithms.h \
    graphgenerator.h \
    benchmarkdialog.h \
    edgedialog.h

FORMS += \
    mainwindow.ui \
    benchmarkdialog.ui \
    edgedialog.ui