#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include "../Engine/Engine.h"
#include <QSslSocket>

QT_REQUIRE_CONFIG(ssl);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    new Engine();
    MainWindow w;



qDebug()<<QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    if (0)
    {
        QPixmap m("./Resource/qtcoin.png");
        QSplashScreen splash(m);
        splash.show();
        splash.finish(&w);
    }
    w.show();
    return a.exec();
}
