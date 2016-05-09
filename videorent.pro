QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = videorent
TEMPLATE = app


SOURCES += \
    main.cpp \
    editordialog.cpp \
    moviedelegate.cpp \
    movietablemodel.cpp

HEADERS  += \
    editordialog.h \
    moviedelegate.h \
    movietablemodel.h
