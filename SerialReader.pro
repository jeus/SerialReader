QT += widgets serialport sql core

TARGET = SerialReader
TEMPLATE = app

HEADERS += \
    dialog.h \
    slavethread.h
SOURCES += \
    main.cpp \
    dialog.cpp \
    slavethread.cpp
INCLUDEPATH += /usr/include/mysql

FORMS +=
