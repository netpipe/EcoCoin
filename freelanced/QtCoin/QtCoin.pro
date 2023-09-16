QT       += core gui sql printsupport

linux {
    DEFINES += zlib QUAZIP DOWNLOAD SMTP SOUND DBUS STORAGE FTP OPENGL
    #DEFINES += DOWNLOAD SMTP SOUND STORAGE FTP
    QT += multimedia svg dbus network opengl
    CONFIG += barcodescan
    #unix:!macx:CONFIG += barcodescan
    barcodescan{
        LIBS+= -lpng -ljpeg
        DEFINES += BARCODE
    }
    //INCLUDEPATH
    message("linux")
}

win32 {
    message("32-bit windows")
    DEFINES += SOUND DBUS DOWNLOAD SMTP STORAGE FTP SMTP
    QT += multimedia svg dbus network
    CONFIG += barcodescan
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Engine/Common.cpp \
    Engine/Engine.cpp \
    Engine/Stylemanager.cpp \
    Engine/crtthread.cpp \
    Engine/dbmanager.cpp \
    Src/QRCode/QrCode.cpp \
    Src/downloadmanager.cpp \
    Src/encryption/simpleCrypt/simplecrypt.cpp \
    Src/ftp-server/dataconnection.cpp \
    Src/ftp-server/ftpcommand.cpp \
    Src/ftp-server/ftpcontrolconnection.cpp \
    Src/ftp-server/ftplistcommand.cpp \
    Src/ftp-server/ftpretrcommand.cpp \
    Src/ftp-server/ftpserver.cpp \
    Src/ftp-server/ftpstorcommand.cpp \
    Src/ftp-server/sslserver.cpp \
    UI/Formftp.cpp \
    UI/Forminvestment.cpp \
    UI/Formwallet.cpp \
    Engine/coinmanager.cpp \
    UI/Frmprogressdlg.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Engine/Common.h \
    Engine/Engine.h \
    Engine/Stylemanager.h \
    Engine/crtthread.h \
    Engine/dbmanager.h \
    Engine/global.h \
    Src/QRCode/QrCode.hpp \
    Src/downloadmanager.h \
    Src/encryption/simpleCrypt/simplecrypt.h \
    Src/ftp-server/dataconnection.h \
    Src/ftp-server/ftpcommand.h \
    Src/ftp-server/ftpcontrolconnection.h \
    Src/ftp-server/ftplistcommand.h \
    Src/ftp-server/ftpretrcommand.h \
    Src/ftp-server/ftpserver.h \
    Src/ftp-server/ftpstorcommand.h \
    Src/ftp-server/sslserver.h \
    UI/Formftp.h \
    UI/Forminvestment.h \
    UI/Formwallet.h \
    Engine/coinmanager.h \
    UI/Frmprogressdlg.h \
    mainwindow.h

FORMS += \
    UI/Formftp.ui \
    UI/Forminvestment.ui \
    UI/Formwallet.ui \
    UI/Frmprogressdlg.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
