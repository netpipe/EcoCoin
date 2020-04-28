#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T19:36:39
#
#-------------------------------------------------

QT       += core gui sql network printsupport  multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqlite-database-example
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/qstylesheetmanager.cpp \
    src/loadtheme.cpp \
    src/downloadmanager.cpp \
    src/encryption/rsa/Rsa.cpp \
    src/encryption/Qt-AES/qaesencryption.cpp

HEADERS  += mainwindow.h \
    src/qstylesheetmanager.h \
    src/loadtheme.h \
    src/downloadmanager.h \
    src/encryption/rsa/Rsa.h \
    src/encryption/Qt-AES/qaesencryption.h \
    src/encryption/encryption.h

FORMS    += mainwindow.ui
