#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T20:47:15
#
#-------------------------------------------------

QT       += core gui
QT       += network

RC_ICONS = icon.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoinChart
TEMPLATE = app

SOURCES += main.cpp \
    dialog.cpp \
    addcoindialog.cpp

HEADERS  += \
    dialog.h \
    addcoindialog.h
