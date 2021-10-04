#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <QStandardItemModel>
//#include <QtCore>
#include <src/coingenerator.h>
#include <src/coinfunctions.h>
#include <src/validatecoins.h>
#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/downloadmanager.h"
#include "src/loadtheme.h"
#include "src/devices.h"
#include "src/admin.h"
#include "src/wallet.h"
#include "src/email.h"
#include "src/ftp-server/ftpgui.h"
#include <src/oglwidget.h>


//references and links
//https://doc.qt.io/qt-5/sql-sqlstatements.html
//https://www.techonthenet.com/mysql/select.php
//http://ismacs.net/singer_sewing_machine_company/why-two-serial-numbers.html some of the first serial numbers
//https://patents.google.com/patent/US3988571A/en

//encrypt with usbdrivename
#ifdef FTP
    FTPGUI *ftpgui;
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    version=0.6;

    ui->setupUi(this);
    qDebug()<<"Application initialized...";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.sqlite");

#ifdef SOUND
    player=new QMediaPlayer();
    playsound("Resource/sounds/ec1_mono.ogg");
#endif

    qDebug() << "db exists" << QDir("db").exists();    if (!QDir("db").exists()){        QDir().mkdir("db");    }

    ui->createyear->setValue(year.toInt());
    ui->createmonth->setCurrentIndex(QDate::currentDate().month()-1);
    ui->maturemonth->setCurrentIndex(QDate::currentDate().month()-1);
    QTime starttime(QTime::currentTime().hour(),QTime::currentTime().minute()); //12:00AM
    ui->createtime->setTime(starttime);

    //load settings
    if( QFileInfo("settings.txt").exists())    {        on_actionOpenCoin_triggered();    }

    getEmailSettings();

    this->setWindowTitle(ui->coinname->text());
    themeInit();

    //set global current year
    int yearvar= QDate::currentDate().year();
    year = QString::number(yearvar);

    //subtract date to set progress bar

    //convert to days then set progressbar
    ui->matureyear->text();
    ui->maturemonth->currentText();

    QDate dNow(QDate::currentDate());
    QDate createdate(ui->createyear->text().toInt(), ui->createmonth->currentIndex()+1, ui->createday->text().toInt());
    QDate maturedate(ui->matureyear->text().toInt(), ui->maturemonth->currentIndex()+1, ui->matureday->text().toInt());
    qDebug() << ui->matureyear->text().toInt()<< ui->maturemonth->currentIndex()+1<< ui->matureday->text().toInt();
    qDebug() << "leap year detector" << QDate::isLeapYear(year.toInt());
    qDebug() << "Today is" << dNow.toString("dd.MM.yyyy")
                << "maturedate is" << maturedate.toString("dd.MM.yyyy")
             << "days to maturing: "
             << dNow.daysTo(maturedate);

   float dayselapsed =  createdate.daysTo(maturedate) - dNow.daysTo(maturedate); // days elapsed since creation
   float daystotalmaturing = createdate.daysTo(maturedate);
   float test4 = dayselapsed / daystotalmaturing * 100;
//   float pi = 3.14;
//   QString b;
//   b.setNum(pi);
  // qDebug() << percent2 << fixed << qSetRealNumberPrecision(2);
    ui->progress->setValue( test4);

    //might have to contact server for ammount available

    ui->progress_2->setValue( 100 );

    //enable these if you want to test rsa+aes encryption in the onpushbutton_3_clicked function.
#ifdef ENCRYPTION
    rsaTester = new Rsa();
    rsaTester->publish_keys(m_e, m_n);
#endif

    if(getkeys() == 1){
      admin=true;
    }else{ //testing
        QString tester1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
        masterkey = GetRandomString(12,tester1.toLatin1());
        coinkey = "testing1234567";
    }

  //  QFile walletdb("wallet.sqlite");
    if(   QFileInfo("wallet.sqlite").exists())    {        walletexists=true;   }

    if (!admin && walletexists){
        //set to client mode
    tabindex=1;
    removedTab = ui->app->widget(tabindex);
    AddRemoveTab(ui->admintab,"Admin",tabindex);
   // AddRemoveTab(ui->admintab,"Admin",tabindex);
    }else{

    }

#ifdef OPENGL
    //  widget = new IrrlichtWidget( ui->tabWidget->findChild<QWidget *>("irrRenderWidget0") );
    //QOpenGLWidget
    OGLWidget *widget2 = new OGLWidget(ui->tabWidget_2->findChild<QWidget *>("openGLWidget"));
    //widget2->(ui->openGLWidget->height());
    // OGLWidget *openGLWidget = new OGLWidget(parent);
    //  openGLWidget->show();
    //ui->openGLWidget->resize(300,400);
    widget2->resize(ui->openGLWidget->width(),ui->openGLWidget->height());
    //  ui->openGLWidget->showMaximized();
    //     setCentralWidget(widget); //widget
    //    showMaximized();
    //  widget2->autoRepaint();

#endif

        //if client only mode
  //  ui->createtime->setTime(starttime);

QDBusConnection connection = QDBusConnection::sessionBus();
connection.registerObject("/qtcoin", this);
connection.registerService("qtcoin.test");
connection.connect("my.qtcoin.dbus", "/qtcoin", "qtcoin.test", "test", this, SLOT(remoteCall(QString)));


//dbus-send --session --type=signal / my.qtcoin.test string:"hello"
//dbus-send --session --type="method_call" --dest=com.user.server /com/user/server com.user.server.function

//    m_hDbus = new DBusHandler();
//    qDebug() << "Createing DBusHandler...\n";
//    m_hDbus->moveToThread(m_hDbus);
//    m_hDbus->start();
//    while (not m_hDbus->isRunning());

}


MainWindow::~MainWindow()
{
    m_hDbus->stop();
    while (m_hDbus->isRunning());

    delete m_hDbus;

    delete ui;
    //QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
}


void MainWindow::playsound(QString test){
#ifdef SOUND
    // player->setMedia(QUrl("qrc:/sounds/ec1_mono.ogg"));
    // player->setMedia(QUrl::fromLocalFile("./paddle_hit.wav"));
     //or play from memory
      QFile file(test.toLatin1());
      file.open(QIODevice::ReadOnly);
      QByteArray* arr = new QByteArray(file.readAll());
      file.close();
      QBuffer* buffer = new QBuffer(arr);
      buffer->open(QIODevice::ReadOnly);
      buffer->seek(0);
//qDebug() << "Media supported state -> " << QMediaPlayer::hasSupport("video/mp4"); // this gives a "1"
      player->setVolume(10);
 //    media->setMedia("sound.mp3");
     player->setMedia(QMediaContent(), buffer);
     player->play();
#endif
}

void          MainWindow::remoteCall(QByteArray message)
{
  std::cout << "Message size: "  << message.size() << std::endl;
}
void          MainWindow::remoteCall(QString message) {
  std::cout << "Message size: "  << message.size() << " data: " << message.toUtf8().constData() << std::endl;
}

void MainWindow::AddRemoveTab(QWidget *tab,QString name,int tabindex){

    // QWidget * test= ui->app->widget(2);
    if (tab->isEnabled()) {
       // QWidget * test= ui->app->widget(2);
        ui->app->removeTab(tabindex);
        tab->setEnabled(false);
    } else {
        tab->setEnabled(true);
        QWidget * test= ui->app->widget(tabindex);
       // ui->app->removeTab(2);
//        if (tab->isVisible()){
            ui->app->insertTab(tabindex,removedTab,name.toLatin1());
//        }
    }
}

void MainWindow::on_actionSyncUSB_triggered()
{
    ListUSB();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_matureradio_yes_clicked()
{
ui->matureradio_no->setChecked(0);
}

void MainWindow::on_encrypted_yes_clicked()
{
    ui->encrypted_no->setChecked(0);
}

void MainWindow::on_matureradio_no_clicked()
{
ui->matureradio_yes->setChecked(1);
}

void MainWindow::on_encrypted_no_clicked()
{
    //check to see if anything is already using encryption
    ui->encrypted_yes->setChecked(1);
}



void MainWindow::on_pushButton_2_clicked() //clearall databases
{
    cleartablesusers();
}

void MainWindow::on_test_clicked()
{
    serverusbtxrx();
    clientusbtxrx();

    QString test = rot13("test");
    qDebug() << "encrypted rot13:" << test;
    qDebug() << "decrypted rot13:" << rot13(test.toLatin1());
}



void MainWindow::on_saveuserinfo_clicked()
{
//Download("ftp://127.0.0.1/");
}

void MainWindow::on_placeCoinsopenfile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.rx *.tx *.txt)"));
    qDebug()<< fileName.toLatin1() ;

    //check extension to see if image to decode qrcode.
    //save qrdata to tmp file set filename and process

    processRXTXfile(fileName);
}

void MainWindow::on_CreateWallet_clicked()
{
  //could create wallet from server generated tx file or send one with first transaction online account registration
qDebug() <<  getHDserial(); //getPSN().toLatin1();
//qDebug () << WordListGenerator(8,"./Resource/wordlists/english.txt");

// get created userinfo file
createWalletTable("");


}

void MainWindow::on_usergenerateQr_clicked()
{
    QString qrstring =  ui->adduserEmail->text().toLatin1()+"::"+
                        ui->addusername->text().toLatin1()+"::"+
                        ui->lineEditPhone->text().toLatin1()+"::"+
                        ui->createuserdatetime->text().toLatin1()+"::"+
                        ui->createextra->text().toLatin1()+"::"+
                        ui->createclass->text().toLatin1();

    GenerateQRCode(qrstring,ui->adduserQRview);

}

void MainWindow::on_addresssearch_clicked()
{
    //    auto model = new QStandardItemModel();
    //ui->treeView->setModel(model);
    //    model->appendRow(new QStandardItem ("test"));
    //        model->appendRow(new QStandardItem ("test"));
    //            model->appendRow(new QStandardItem ("test"));

    //QList<QStandardItem*> test;
    //QStandardItem *test34 =new QStandardItem ("test");
    //test.append(test34);
    //        model->item(1)->appendRows(test);
    //        ui->treeView->expandAll();


}



void MainWindow::on_adminmode_clicked()
{
    // if no multiple wallet support then warn user it will erase wallet and to back it up to safe location

    AddRemoveTab(ui->admintab,"Admin",tabindex);

}

void MainWindow::on_ftpserver_clicked()
{
#ifdef FTP
    if (adminftp==0){
    ftpgui = new FTPGUI;
    adminftp=1;
    }
    if (adminftp) { ftpgui->show();}
#endif
}

void MainWindow::on_CheckAvailability_clicked()
{

    //download info file from server
    manager.Download("ftp://admin:qt@127.0.0.1:8258/available");
}

void MainWindow::on_updateVerify_clicked()
{
    //download all databases for verification of coins offline - still succeptable to double spend but atleast its able to work without server.
    //possibly place usersigned coins they would like to spend that day into a pickup database for the server to hold for them temporary holding when doing offline tx's
manager.Upload("ftp://127.0.0.1","admin","qt","8258","./ftp.ini");
}
