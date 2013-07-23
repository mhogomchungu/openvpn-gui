#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T22:53:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openvpn-gui
TEMPLATE = app


SOURCES += main.cpp\
	openvpn.cpp \
    logwindow.cpp

HEADERS  += openvpn.h \
    logwindow.h

FORMS    += \
    logwindow.ui


INCLUDEPATH += /home/local/KDE4/include /usr/include /home/ink/src/openvpn-gui/build

LIBS += -lkdeui -L/home/local/KDE4/lib -L/usr/lib
