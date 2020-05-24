#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T19:36:39
#
#-------------------------------------------------

QT += core gui sql printsupport

#emscripten
linux {
DEFINES += SOUND DBUS DOWNLOAD SMTP STORAGE
QT += multimedia svg dbus network
CONFIG += barcodescan
#unix:!macx:CONFIG += barcodescan
barcodescan{
LIBS+= -lpng -ljpeg
DEFINES += BARCODE
}

}

win32 {
DEFINES += SOUND DBUS DOWNLOAD SMTP STORAGE
QT += multimedia svg dbus network
CONFIG += barcodescan
}

wasm-emscripten {
LIBS += -lidbfs.js -lnodefs.js -lworkerfs.js
#-lnodefs.js, -lidbfs.js', -lworkerfs.js, -lproxyfs.js
}


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
    src/encryption/simpleCrypt/simplecrypt.cpp \
    src/smtp/smtpclient.cpp \
    src/smtp/quotedprintable.cpp \
    src/smtp/mimetext.cpp \
    src/smtp/mimepart.cpp \
    src/smtp/mimemultipart.cpp \
    src/smtp/mimemessage.cpp \
    src/smtp/mimeinlinefile.cpp \
    src/smtp/mimehtml.cpp \
    src/smtp/mimefile.cpp \
    src/smtp/mimecontentformatter.cpp \
    src/smtp/mimeattachment.cpp \
    src/smtp/emailaddress.cpp \
    src/QRCode/QrCode.cpp \
    src/Barcode/functii.cpp \
    src/Barcode/quirc/lib/decode.c \
    src/Barcode/quirc/lib/identify.c \
    src/Barcode/quirc/lib/quirc.c \
    src/Barcode/quirc/lib/version_db.c

HEADERS  += mainwindow.h \
    src/qstylesheetmanager.h \
    src/loadtheme.h \
    src/downloadmanager.h \
    src/encryption/rsa/Rsa.h \
    src/encryption/Qt-AES/qaesencryption.h \
    src/encryption/encryption.h \
    src/smtp/SmtpMime \
    src/smtp/smtpexports.h \
    src/smtp/smtpclient.h \
    src/smtp/quotedprintable.h \
    src/smtp/mimetext.h \
    src/smtp/mimepart.h \
    src/smtp/mimemultipart.h \
    src/smtp/mimemessage.h \
    src/smtp/mimeinlinefile.h \
    src/smtp/mimehtml.h \
    src/smtp/mimefile.h \
    src/smtp/mimecontentformatter.h \
    src/smtp/mimeattachment.h \
    src/smtp/emailaddress.h \
    src/encryption/simpleCrypt/simplecrypt.h \
    src/coinfunctions.h \
    src/validatecoins.h \
    src/coingenerator.h \
    src/QRCode/QrCode.hpp \
    src/Barcode/functii.h \
    src/dbus/dbushandler.h \
    src/Barcode/quirc/lib/quirc.h \
    src/Barcode/quirc/lib/quirc_internal.h \
    src/Barcode/quirc/tests/dbgutil.h \
    src/Barcode/quirc/tests/inspect.h \
    src/admin.h \
    src/wallet.h \
    src/email.h


FORMS    += mainwindow.ui

RESOURCES +=

DISTFILES += \
    qtCoin.ico
