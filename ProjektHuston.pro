QT += core gui network

TARGET = projekt_huston
TEMPLATE = app

INCLUDEPATH += /include/SDL/ \
               /lib

LIBS += SDL.dll

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Connection.cpp \
    ControllerData.cpp

HEADERS += \
    MainWindow.h \
    Connection.h \
    ControllerData.h \
    Defines.h
