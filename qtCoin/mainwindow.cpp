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
#ifdef SMTP
#include "src/smtp/SmtpMime"
#endif
#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/downloadmanager.h"
#include "src/loadtheme.h"

#include "src/admin.h"
#include "src/wallet.h"



//references and links
//https://doc.qt.io/qt-5/sql-sqlstatements.html
//https://www.techonthenet.com/mysql/select.php
//http://ismacs.net/singer_sewing_machine_company/why-two-serial-numbers.html some of the first serial numbers
//https://patents.google.com/patent/US3988571A/en

//encrypt with usbdrivename

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
    QFile Fout("settings.txt");    if(Fout.exists())    {        on_actionOpenCoin_triggered();    }    Fout.close();
    this->setWindowTitle(ui->coinname->text());
    themeInit();

    //set global current year
    int yearvar= QDate::currentDate().year();
    year = QString::number(yearvar);

    //subtract date to set progress bar

    //    QString s = QDate::currentDate().toString();
    //    QDate::currentDate().day();
    //    QDate::currentDate().month();
    //    QDate::currentDate().year();

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

    //enable these if you want to test rsa+aes encryption in the onpushbutton_3_clicked function.
//    rsaTester = new Rsa();
//    rsaTester->publish_keys(m_e, m_n);

    if(getkeys() == 1){
      //
    }else{ //testing
        QString tester1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
        masterkey = GetRandomString(12,tester1.toLatin1());
        coinkey = "testing1234567";
    }
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
    //if client only mode

//ui->settingstab->setEnabled(false);
//ui->settingstab->setVisible(false);
//   QWidget * test= ui->app->widget(2);
//ui->app->removeTab(2);
//ui->app->insertTab(2,test,"Settings");

  //  ui->createtime->setTime(starttime);

}


MainWindow::~MainWindow()
{
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

      player->setVolume(10);
 //    media->setMedia("sound.mp3");
     player->setMedia(QMediaContent(), buffer);
     player->play();
#endif
}



void MainWindow::BackUptoUSB(){
#ifdef STORAGE
    //store and retrieve master encryption keys with this.

//https://stackoverflow.com/questions/40035332/how-to-get-path-to-usb-drive-on-linux-in-qt
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {

       qDebug() << storage.rootPath();

//       QString storagestring=storage.rootPath();
//       QRegExp rx("[/]");// match a comma or a space
//       QStringList list2 = storagestring.split(rx);

//      qDebug() << storagestring.at(3);
       QString usbstring = "backupdevice";//ui->usbdrivename->text().toLatin1();

        if (storage.rootPath().contains(usbstring)){
            //qDebug() << "yep" << "/n";
            backupusbpath = storage.rootPath().contains(usbstring);

            if (storage.isReadOnly())
                qDebug() << "isReadOnly:" << storage.isReadOnly();

//                qDebug() << "name:" << storage.name();
//                qDebug() << "fileSystemType:" << storage.fileSystemType();
//                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
        } else {
            backupusbpath="";
        }

        if (backupusbpath.toLatin1() == "")
        {
            //date
            QFile::copy("/settings.txt", backupusbpath.toLatin1() );
            QFile::copy("/coins.sqlite", backupusbpath.toLatin1() );
            QFile::copy("/availableCoins.sqlite", backupusbpath.toLatin1() );
            QFile::copy("/hashes.txt", backupusbpath.toLatin1() );

            QMessageBox Msgbox;
                Msgbox.setText("drive not found: ");
                Msgbox.exec();
        }
     }
#endif
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
    QString test = rot13("test");
    qDebug() << "encrypted rot13:" << test;
    qDebug() << "decrypted rot13:" << rot13(test.toLatin1());
}

void MainWindow::on_smtpsave_clicked()
{

}

void MainWindow::on_smtptestmessage_clicked()
{

}


void MainWindow::on_saveuserinfo_clicked()
{

}

void MainWindow::on_placeCoinsopenfile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.rx *.tx *.txt)"));
    qDebug()<< fileName.toLatin1() ;

    //check extension to see if image to decode qrcode.
    //save qrdata to tmp file set filename and process

    processRXTXfile(fileName);
}



int MainWindow::smtpsend(QString toemail,QString Message){
#ifdef SMTP
    bool ssl;
    if ( ui->smtpssl->isChecked() == 1 ){ ssl = 1;    }else{ ssl = 0; }

    SmtpClient smtp (ui->smtphost->text().toLatin1(), ui->smtpport->text().toInt(), ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);
    smtp.setUser(ui->smtpemail->text());
    smtp.setPassword(ui->smtppassword->text());

    MimeMessage message;
    EmailAddress sender(ui->smtpemail->text(), ui->smtpemail->text()); //email,name
    message.setSender(&sender);
    EmailAddress to(toemail.toLatin1(), toemail.toLatin1()); // email, name
    message.addRecipient(&to);
    message.setSubject(ui->coinname->text());

    MimeText text;
  //  text.setText("Hi,\nThis is a simple email message.\n");
    text.setText(Message.toLatin1());
    // Now add it to the mail
    message.addPart(&text);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
        return 1;
    }

    if (!smtp.login()) {
        qDebug() << "Failed to login!" << endl;
        return 2;
    }

    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;
        return 3;
    }
    smtp.quit();
#endif
}

void MainWindow::on_CreateWallet_clicked()
{

  //could create wallet from server generated tx file or send one with first transaction online account registration


//qDebug () << WordListGenerator(8,"./Resource/wordlists/english.txt");

}

