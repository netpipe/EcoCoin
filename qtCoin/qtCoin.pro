#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T19:36:39
#
#-------------------------------------------------

QT       += core gui sql network printsupport  multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtCoin
TEMPLATE = app

RC_FILE = ./qtCoin.rc
ICON = ./qtCoin.ico

#CONFIG += c++11
CONFIG += console

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    src/qstylesheetmanager.cpp \
    src/downloadmanager.cpp \
    src/encryption/rsa/Rsa.cpp \
    src/encryption/Qt-AES/qaesencryption.cpp \
    src/encryption/simpleCrypt/simplecrypt.cpp

HEADERS  += mainwindow.h \
    src/qstylesheetmanager.h \
    src/loadtheme.h \
    src/downloadmanager.h \
    src/encryption/rsa/Rsa.h \
    src/encryption/Qt-AES/qaesencryption.h \
    src/encryption/encryption.h \
    coingenerator.h \
    src/encryption/simpleCrypt/simplecrypt.h \
    src/coinfunctions.h

FORMS    += mainwindow.ui

RESOURCES += \
    itas_js.qrc

DISTFILES += \
    qtCoin.ico
