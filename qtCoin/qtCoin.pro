#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T19:36:39
#
#-------------------------------------------------

QT += core gui sql printsupport
LIBS += -lglut -lGL -lGLU

#emscripten
linux {
DEFINES += zlib QUAZIP DOWNLOAD SMTP SOUND DBUS STORAGE FTP OPENGL
QT += multimedia svg dbus network opengl
CONFIG += barcodescan
#unix:!macx:CONFIG += barcodescan
barcodescan{
LIBS+= -lpng -ljpeg
DEFINES += BARCODE
}

}

win32 {
DEFINES += SOUND DBUS DOWNLOAD SMTP STORAGE FTP zlib QUAZIP
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
    src/Barcode/quirc/lib/version_db.c \
    src/ftp-server/dataconnection.cpp \
    src/ftp-server/debuglogdialog.cpp \
    src/ftp-server/ftpcommand.cpp \
    src/ftp-server/ftpcontrolconnection.cpp \
    src/ftp-server/ftplistcommand.cpp \
    src/ftp-server/ftpretrcommand.cpp \
    src/ftp-server/ftpserver.cpp \
    src/ftp-server/ftpstorcommand.cpp \
    src/ftp-server/sslserver.cpp \
    src/ftp-server/ftpgui.cpp \
    src/quazip/JlCompress.cpp \
    src/quazip/qioapi.cpp \
    src/quazip/quaadler32.cpp \
    src/quazip/quacrc32.cpp \
    src/quazip/quagzipfile.cpp \
    src/quazip/quaziodevice.cpp \
    src/quazip/quazip.cpp \
    src/quazip/quazipdir.cpp \
    src/quazip/quazipfile.cpp \
    src/quazip/quazipfileinfo.cpp \
    src/quazip/quazipnewinfo.cpp \
    src/quazip/unzip.c \
    src/quazip/zip.c \
    src/zlib/adler32.c \
    src/zlib/compress.c \
    src/zlib/crc32.c \
    src/zlib/deflate.c \
    src/zlib/gzclose.c \
    src/zlib/gzlib.c \
    src/zlib/gzread.c \
    src/zlib/gzwrite.c \
    src/zlib/infback.c \
    src/zlib/inffast.c \
    src/zlib/inflate.c \
    src/zlib/inftrees.c \
    src/zlib/trees.c \
    src/zlib/uncompr.c \
    src/zlib/zutil.c \
    src/oglwidget.cpp

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
    src/email.h \
    src/ssl.h \
    src/devices.h \
    src/ftp-server/dataconnection.h \
    src/ftp-server/debuglogdialog.h \
    src/ftp-server/ftpcommand.h \
    src/ftp-server/ftpcontrolconnection.h \
    src/ftp-server/ftplistcommand.h \
    src/ftp-server/ftpretrcommand.h \
    src/ftp-server/ftpserver.h \
    src/ftp-server/ftpstorcommand.h \
    src/ftp-server/sslserver.h \
    src/ftp-server/ftpgui.h \
    src/quazip/ioapi.h \
    src/quazip/JlCompress.h \
    src/quazip/quaadler32.h \
    src/quazip/quachecksum32.h \
    src/quazip/quacrc32.h \
    src/quazip/quagzipfile.h \
    src/quazip/quaziodevice.h \
    src/quazip/quazip.h \
    src/quazip/quazipdir.h \
    src/quazip/quazipfile.h \
    src/quazip/quazipfileinfo.h \
    src/quazip/quazip_global.h \
    src/quazip/quazipnewinfo.h \
    src/quazip/unzip.h \
    src/quazip/zip.h \
    src/zlib/crc32.h \
    src/zlib/deflate.h \
    src/zlib/gzguts.h \
    src/zlib/inffast.h \
    src/zlib/inffixed.h \
    src/zlib/inflate.h \
    src/zlib/inftrees.h \
    src/zlib/trees.h \
    src/zlib/zconf.h \
    src/zlib/zlib.h \
    src/zlib/zutil.h \
    src/oglwidget.h


FORMS    += mainwindow.ui \
    src/ftp-server/debuglogdialog.ui \
    src/ftp-server/ftpgui.ui

RESOURCES +=

DISTFILES += \
    qtCoin.ico
