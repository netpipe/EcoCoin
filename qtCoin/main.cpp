#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include "src/qstylesheetmanager.h"
#include <QThread>
#include <QDebug>
//https://github.com/ganquan0910/qt5-sqlite-connection-example

class I : public QThread
{
public:
    static void sleep(unsigned long secs) { QThread::sleep(secs); }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStyleSheetManager::loadStyleSheet("Resource/themes/style4.qss");
//    QFile file("style4.qss");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());



        QPixmap m("./Resource/qtcoin.png");


    MainWindow w;

    QSplashScreen splash(m);
    splash.show();

    splash.finish(&w);
    //     QThread::sleep(10);
  //  I::sleep(2); // splash is shown for 5 seconds
    w.show();

    return a.exec();
}
