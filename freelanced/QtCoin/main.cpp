#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include "Engine/Engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    new Engine();
    MainWindow w;

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
