QT       += core gui sql printsupport

linux {
    DEFINES += zlib QUAZIP DOWNLOAD SOUND DBUS STORAGE FTP OPENGL CLIENT_APP
    QT += multimedia svg dbus network opengl
    CONFIG += barcodescan
    #unix:!macx:CONFIG += barcodescan
    barcodescan{
        LIBS+= -lpng -ljpeg
        DEFINES += BARCODE
    }
}

win32 {
    DEFINES += SOUND DBUS DOWNLOAD STORAGE FTP SMTP CLIENT_APP
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
    ../Engine/Common.cpp \
    ../Engine/Engine.cpp \
    ../Engine/Stylemanager.cpp \
    ../Engine/crtthread.cpp \
    ../Engine/dbmanager.cpp \
    ../Engine/coinmanager.cpp \
    ../Src/QRCode/QrCode.cpp \
    ../Src/downloadmanager.cpp \
    ../Src/encryption/simpleCrypt/simplecrypt.cpp \
    ../Src/ftp-server/dataconnection.cpp \
    ../Src/ftp-server/ftpcommand.cpp \
    ../Src/ftp-server/ftpcontrolconnection.cpp \
    ../Src/ftp-server/ftplistcommand.cpp \
    ../Src/ftp-server/ftpretrcommand.cpp \
    ../Src/ftp-server/ftpserver.cpp \
    ../Src/ftp-server/ftpstorcommand.cpp \
    ../Src/ftp-server/sslserver.cpp \
    ../Src/smtp/emailaddress.cpp \
    ../Src/smtp/mimeattachment.cpp \
    ../Src/smtp/mimecontentformatter.cpp \
    ../Src/smtp/mimefile.cpp \
    ../Src/smtp/mimehtml.cpp \
    ../Src/smtp/mimeinlinefile.cpp \
    ../Src/smtp/mimemessage.cpp \
    ../Src/smtp/mimemultipart.cpp \
    ../Src/smtp/mimepart.cpp \
    ../Src/smtp/mimetext.cpp \
    ../Src/smtp/quotedprintable.cpp \
    ../Src/smtp/smtpclient.cpp \
    UI/Formftp.cpp \
    UI/Formwallet.cpp \
    UI/Frmprogressdlg.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    ../Engine/Common.h \
    ../Engine/Engine.h \
    ../Engine/Stylemanager.h \
    ../Engine/crtthread.h \
    ../Engine/dbmanager.h \
    ../Engine/global.h \
    ../Engine/coinmanager.h \
    ../Src/QRCode/QrCode.hpp \
    ../Src/downloadmanager.h \
    ../Src/encryption/simpleCrypt/simplecrypt.h \
    ../Src/ftp-server/dataconnection.h \
    ../Src/ftp-server/ftpcommand.h \
    ../Src/ftp-server/ftpcontrolconnection.h \
    ../Src/ftp-server/ftplistcommand.h \
    ../Src/ftp-server/ftpretrcommand.h \
    ../Src/ftp-server/ftpserver.h \
    ../Src/ftp-server/ftpstorcommand.h \
    ../Src/ftp-server/sslserver.h \
    ../Src/smtp/emailaddress.h \
    ../Src/smtp/mimeattachment.h \
    ../Src/smtp/mimecontentformatter.h \
    ../Src/smtp/mimefile.h \
    ../Src/smtp/mimehtml.h \
    ../Src/smtp/mimeinlinefile.h \
    ../Src/smtp/mimemessage.h \
    ../Src/smtp/mimemultipart.h \
    ../Src/smtp/mimepart.h \
    ../Src/smtp/mimetext.h \
    ../Src/smtp/quotedprintable.h \
    ../Src/smtp/smtpclient.h \
    ../Src/smtp/smtpexports.h \
    UI/Formftp.h \
    UI/Formwallet.h \    
    UI/Frmprogressdlg.h \
    mainwindow.h \

FORMS += \
    UI/Formftp.ui \
    UI/Formwallet.ui \
    UI/Frmprogressdlg.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
