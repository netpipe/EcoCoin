#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/downloadmanager.h"
#include "src/loadtheme.h"
#include <QFileDialog>
#include <coingenerator.h>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>

#include "./src/smtp/SmtpMime"

//#include <QtCore>

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

    //createUserTable();
    //selectUsers();

    player=new QMediaPlayer();
   // player->setMedia(QUrl("qrc:/sounds/ec1_mono.ogg"));
   // player->setMedia(QUrl::fromLocalFile("./paddle_hit.wav"));
    //or play from memory
     QFile file("Resource/sounds/ec1_mono.ogg");
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


ui->smtppassword->setEchoMode(QLineEdit::Password);

    ui->createyear->setValue(year.toInt());
    //if (QDate::currentDate().month()="January")

    //ui->createmonth->(QDate::currentDate().month());
ui->createmonth->setCurrentIndex(QDate::currentDate().month()-1);
ui->maturemonth->setCurrentIndex(QDate::currentDate().month()-1);

QTime starttime(QTime::currentTime().hour(),QTime::currentTime().minute()); //12:00AM

ui->createtime->setTime(starttime);

    //load settings
    QFile Fout("settings.txt");
    if(Fout.exists())
    {
        on_actionOpenCoin_triggered();
    }
    Fout.close();

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

    //int pcreate = ui->createmonth->currentIndex()+1;
    int pmature = ui->maturemonth->currentIndex()+1;


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
    qDebug() << test4;
    ui->progress->setValue( test4);

    //ui->createmonth->setValue();comboBox->currentIndex();
  //  qDebug()<< pcreate*24;

    rsaTester = new Rsa();
    rsaTester->publish_keys(m_e, m_n);


    //testing
    masterkey = "testing";
    coinkey = "testing1234567";


    QString yeardb = "2020ajasjdkl";
  // yeardb= yeardb.mid(0,4);
   yeardb= yeardb.left(4);
   qDebug() << yeardb;


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

void MainWindow::SQLTest(QString dbname,QString Query)
{
    db.setDatabaseName(dbname.toLatin1());
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }


    QString query;
    query.append(Query.toLatin1());

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(0).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(""));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(""));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(""));
        row++;
    }

    query.clear();
    db.close();
}

void MainWindow::serverusbtxrx(){
    //automatic function to do rxtx from usb for cold storage


    //export db's and overwrite if valid

}

void MainWindow::clientusbtxrx(){

    //import db's and overwrite if valid

}

void MainWindow::ListUSB(){
    //store and retrieve master encryption keys with this.

//https://stackoverflow.com/questions/40035332/how-to-get-path-to-usb-drive-on-linux-in-qt
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {

       qDebug() << storage.rootPath();

//       QString storagestring=storage.rootPath();
//       QRegExp rx("[/]");// match a comma or a space
//       QStringList list2 = storagestring.split(rx);

//      qDebug() << storagestring.at(3);
       QString usbstring = ui->usbdrivename->text().toLatin1();


        if (storage.rootPath().contains(usbstring)){
            //qDebug() << "yep" << "/n";
            usbpath = storage.rootPath().contains(usbstring);

            if (storage.isReadOnly())
                qDebug() << "isReadOnly:" << storage.isReadOnly();

                qDebug() << "name:" << storage.name();
                qDebug() << "fileSystemType:" << storage.fileSystemType();
                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
        } else {
            usbpath="";
        }

        if (usbpath.toLatin1() == "")
        {
            QMessageBox Msgbox;
                Msgbox.setText("drive not found: ");
                Msgbox.exec();
        }
     }
}

void MainWindow::BackUptoUSB(){
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

                qDebug() << "name:" << storage.name();
                qDebug() << "fileSystemType:" << storage.fileSystemType();
                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
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
}

void MainWindow::searchyearly(QString ownerID)
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;
    query.append("SELECT * FROM "+ownerID);

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }

    query.clear();
    db.close();
}

void MainWindow::cleartablesusers()
{
    // removes databases/users to start fresh
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure ?", "remova all tables/users ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
      QFile::remove("./db/"+year+".sqlite");

      QFile::remove("coins.txt");
      QFile::remove("coins.sqlite");
      QFile::remove("availableCoins.sqlite");
      QFile::remove("rcoins.sqlite");
      QFile::remove("hashes.txt");
   //   QApplication::quit();
    } else {
      qDebug() << "no";
      return;
    }

}
void MainWindow::createyearly(QString eownerID)
{
    //holds users generated from each new year and their coins pulled from rcoins.sqlite

    db.setDatabaseName("./db/"+year+".sqlite");

    if(db.open())    {       qDebug()<<"Successful database connection";    }
    else    {       qDebug()<<"Error: failed database connection";    }

    QString query;

    qDebug() << "test" << eownerID.toLatin1();

    query.append("CREATE TABLE IF NOT EXISTS ""'"+eownerID.toLatin1()+"'""("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "addr VARCHAR(100),"
                    "datetime INTEGER,"
                    "class INTEGER"
                    ");");


    QSqlQuery create;
    create.prepare(query);

    if (create.exec())
    {
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }
    query.clear();
    db.close();
}



void MainWindow::createUserTable()
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }
    QString query;

    query.append("CREATE TABLE IF NOT EXISTS users("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "userid VARCHAR(100),"
                    "name VARCHAR(100),"
                    "password VARCHAR(100),"
                    "phone INTEGER,"//                    "phone INTEGER NOT NULL,"
                     "datetime INTEGER NOT NULL,"
                     "ekey VARCHAR(100),"
                      "total VARCHAR(100),"
                     "extra VARCHAR(100),"
                    "class INTEGER"
                    ");");


    QSqlQuery create;
    create.prepare(query);

    if (create.exec())
    {
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }
    query.clear();
    db.close();
}

void MainWindow::insertUser() //strictly a db to hold all userid's for verification
{
    createUserTable();

    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;

      QString euserid = ui->lineEditName->text();

//    QByteArray bFname = EncryptMsg(ui->lineEditName->text(),"123456789", "your-IV-vector");
//    QString mykey1 = BigInt2Str(m_e); //rsa keys
//    QString mykey2 = BigInt2Str(m_n); //rsa keys

    query.append("INSERT INTO users("
                 "userid,"
                    "name,"
                    "password,"
                    "phone,"//
                 "datetime,"
                 "ekey,"
                 "total,"
                 "extra,"
                    "class)"
                    "VALUES("
                    "'"+euserid.toLatin1()+"',"
                    "'"+ui->lineEditSurname->text().toLatin1()+"',"
                    "'"+ui->lineEditPassword->text().toLatin1()+"',"
                  "'"+ui->lineEditPhone->text().toLatin1()+"',"
                "'"+ui->createuserdatetime->text()+"',"
                 "'""'," //ekey
                 "'""'," //ammount
                    "'"+ui->createextra->text().toLatin1()+"',"
                 "'"+ui->createclass->text()+"'"
                    ");");

     qDebug()<< euserid.toLatin1();

    QSqlQuery insert;
    insert.prepare(query);

    if (insert.exec())
    {
        qDebug()<<"The user is properly inserted";
    }
    else
    {
        qDebug()<<"The user is not inserted correctly";
        qDebug()<<"ERROR! "<< insert.lastError();
    }

    query.clear();
    db.close();

}

void MainWindow::selectUsers()
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }


    QString query;
    query.append("SELECT * FROM users");

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }

    query.clear();
    db.close();
}

void MainWindow::on_pushButtonInsertUser_clicked()
{
    //QString temp = GenerateClientAddress(8);
    QString temp = GetRandomString(8,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
    ui->lineEditName->setText(year.toLatin1()+temp.toLatin1());
    //ui->lineEditName->setText(temp.toLatin1()); //testing
    QString ownerid=ui->lineEditName->text().toLatin1();
    QString password=ui->lineEditPassword->text();

    //fix later
    if (validateID(ownerid) == 0 ){
    for (int i=0;i < 100 ; i++) { //100 tries
        if (validateID(ownerid) == 1 ){

        }else {
            break;
        }
        QString temp = GetRandomString(8,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
        ownerid=ui->lineEditName->text().toLatin1();
    }
    }

// use password first to make more secure so no need to store in plaintext
 //   QString crypted = simplecrypt(ownerid.toLatin1(),ui->lineEditPassword->text(),QCryptographicHash::Sha512);
  //  QString crypted2 = simplecrypt(crypted.toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512);
         qDebug() << ownerid.toLatin1() ;
    QString crypted2 = simplecrypt(ownerid.toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512);
   // QString decrypted = simpledecrypt(crypted,"test2",QCryptographicHash::Sha512);
     qDebug() << crypted2 ;
//  crypted2 = simplecrypt(ownerid.toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512);
// qDebug() << crypted2 ;
//   crypted2 = simplecrypt(ownerid.toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512);
//  qDebug() << crypted2 ;
    ui->lineEditName->setText(crypted2.toLatin1());

    createyearly(crypted2);

  //  createyearly(crypted2); //the /n causes issues

  //  qDebug() << "lineeditname " << ui->lineEditName->text();

    insertUser();




    //selectUsersCoins(temp.toLatin1(),year.toLatin1());

    //combine user year+userid to give to user
//ui->createuserdatetime->text();
//ui->createuserdatetime->setText();

    ui->lineEditName->setText(ownerid.toLatin1());
    ui->lineEditName->setEnabled(1);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lineEditName->text());

    // selectUsers(); //refresh user table

}



void MainWindow::on_actionSyncUSB_triggered()
{
    ListUSB();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_3_clicked() //search button
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }
        QString query;

        //testing save the keys maybe shorten the encryption length ?

    if (ui->encrypted_yes->text() == "Yes" ){
            QByteArray bFname = EncryptMsg(ui->userid->text(),"123456789", "your-IV-vector");
            QString mykey1 = BigInt2Str(m_e); //rsa keys
            QString mykey2 = BigInt2Str(m_n); //rsa keys

            query.append("SELECT * FROM users WHERE name =" "'" + bFname  + "'" );

    }else {
        query.append("SELECT * FROM users WHERE name =" "'" + ui->userid->text()  + "'" );
}

    //search for coin owner / validity

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    QString mykey1 = BigInt2Str(m_e); //rsa keys
    QString mykey2 = BigInt2Str(m_n); //rsa keys

    if (ui->encrypted_yes->text() == "Yes" ){

    while (select.next())
    {
        Rsa *rsa = new Rsa(BigInt(mykey1.toStdString()), BigInt(mykey2.toStdString()));
        QString strMsg = DecryptMsg(select.value(1).toByteArray().constData(), rsa,"123456789", "your-IV-vector");
       // QString strDate = DecryptMsg(bFname, rsa,"123456789", "your-IV-vector");
        delete rsa;

        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(strMsg));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }
}else{
    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }
    }
    query.clear();
    db.close();
}

void MainWindow::on_btnApply_clicked() //theme
{
    if (ui->cmbTheme->currentText().toLatin1() != ""){
        QStyleSheetManager::loadStyleSheet( ui->cmbTheme->currentText().toLatin1());
    }
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
ui->encrypted_no->setChecked(1);
}

void MainWindow::on_encrypted_no_clicked()
{
    //check to see if anything is already using encryption
    ui->encrypted_yes->setChecked(1);
}



void MainWindow::placeCoins(QString euserid,QString ammount) //free coins from coins.db
{
// place into client walled based on yearly tables when created ID

///////////////////
    //https://doc.qt.io/qt-5/qcryptographichash.html
//    QString test=md5Checksum("testing123");//
//    qDebug() << "md5sum:" << test.toLatin1();

//    QByteArray array;
//    array.append("ABCDEF12343");
//    qDebug() << QString(array.toHex());

//////////////////////

//ui->givecoinsammount.text().toLatin1()
    //pull from rcoins db and remove coin after placing in tmp text file
//QString coin;
        QVariantList coins;
    db.setDatabaseName("rcoins.sqlite");
    db.open();
        QSqlQuery query;
        query.exec("SELECT * FROM coins ORDER RAND() LIMIT "+ammount);
        while (query.next()) {
        coins << query.value(0).toString();
  //  place into textfile
        }
    db.close();


//verify coins and insert into yearly userid
QVariantList signedcoins;

for (int i; i > coins.length(); i++){
//if (coin > 8) // coin not from rcoins needs decryption first
    signedcoins << validateCOINsign( coins.at(i).toString(), euserid.toLatin1() );

//  // if ( validateCOINsign(coin) == "valid"){
//        qDebug() << "coin is valid"
//}else {  qDebug << signedcoin; };

}

    //valid user so it must have table already in yeardb
    qDebug()<< "inserting coins into yeardb";
    //sqlite create randomized availablecoins
    db.setDatabaseName("./"+ yeardb +".sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }
    db.transaction();

    QString query2 = "INSERT INTO ""'"+euserid+"'""(origid,addr) VALUES (?,?)";

//    qDebug() << query;
    QSqlQuery insert;
    insert.prepare(query2);
  //  insert.addBindValue(index);
    insert.addBindValue(coins);

    if(insert.execBatch())
    {
        qDebug() << "Coin is properly inserted";
    }
    else
    {
        qDebug()<<"ERROR! "<< insert.lastError();
        //undo
    }
    db.commit();
   // index.clear();
    coins.clear();
    insert.clear();



    //not sure what this is for yet
 //    db.setDatabaseName("./"+ result +".sqlite");
       db.open();
           QSqlDatabase::database().transaction();
           QSqlQuery query3;
         //  query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
           while (query3.next()) {
               int employeeId = query3.value(0).toInt();
             //  rcoins <<      //decrypt coins and reencrypt for new user
               //can place into text file to be sure then delete here// verify enough is available

              // query.exec("DELETE FROM userid WHERE addr ="); //do this after the tx has been validated if sending from user to user
                         //if sending from admin pull from rcoins

               //   "DELETE FROM euserid WHERE addr = "+ "OR StudentId = 12;"
                  // "UPDATE coins SET lastupdated WHERE userid=11"
                  // "DROP table" +userid
                  // "ALTER TABLE name ADD COLUMN test TEXT" or char(50)
                     // "ALTER TABLE name DROP COLUMN name"
           }
           QSqlDatabase::database().commit();
       db.close();

}

void MainWindow::on_placeCoins_clicked()
{
    //if any incorrect flag account for checking also disable other transactions.
    int verified = 0;//md5verifydb();

    placeCoins("1","1");

    if (verified == 1){
    QMessageBox Msgbox;
        Msgbox.setText("coins sent ");
        Msgbox.exec();
    }
}

void MainWindow::generateTXfile(QString euserid,QString etxcoins){ //file to send from client
    //encrypt with masterkey encrypted userID and user encryption key and datetime to validate coins from their wallet

    //might only need ammount and userid and other usersid for tx's because users are not sending coins directly to other users

 //   db.setDatabaseName("./"+ result +".sqlite");
       db.open();
           QSqlDatabase::database().transaction();
           QSqlQuery query;
         //  query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
           while (query.next()) {
               int employeeId = query.value(0).toInt();
           }
           QSqlDatabase::database().commit();
       db.close();

       QFile MyFile("rx.txt");
       MyFile.open(QIODevice::ReadWrite);
       QTextStream in (&MyFile);
       QString line;
       QStringList list;
        //   QList<QString> nums;
       QStringList nums;
       QRegExp rx("[:]");
       do {
           line = in.readLine();
           if (line.contains(":")) {
               list = line.split(rx);
               nums.append(list.at(1).toLatin1());
           }
       } while (!line.isNull());

       //append md5sum

}

void MainWindow::generateRXfile(QString euserid,QString etxcoins){ //rxfile to give client encrypted coins to put in wallet might not be needed for client only cold server

    //etxcoins is either tmptxtfile with encryptedcoins or to make more secure use memory

    //encrypt coin addresses with mastercoinkey and userid to send the coins to their wallets if so desired (upto the user for extra security also gives ability to send between user accounts)

    //verify with time encrypted password verify and send time info password encrypted with userid

    // do they get their actual userid or an encrypted version based on masterkey and their password

    //include public address of person who youd like to send to or have it cashed to paypal/cheque or some other deposit service
 //   db.setDatabaseName("./"+ result +".sqlite");

       db.open();
           QSqlDatabase::database().transaction();
           QSqlQuery query;
         //  query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
           while (query.next()) {
               int employeeId = query.value(0).toInt();
             //  rcoins <<      //decrypt coins and reencrypt for new user
               //can place into text file to be sure then delete here// verify enough is available
           }
           QSqlDatabase::database().commit();
       db.close();

       QFile MyFile("tx.txt");
       MyFile.open(QIODevice::ReadWrite);
       QTextStream in (&MyFile);
       QString line;
       QStringList list;
        //   QList<QString> nums;
       QStringList nums;
       QRegExp rx("[:]");
       do {
           line = in.readLine();
           if (line.contains(":")) {
               list = line.split(rx);
               nums.append(list.at(1).toLatin1());
           }
       } while (!line.isNull());

       //append md5sum


}

QString MainWindow::validateCOINsign(QString coin,QString euserID){ // for getting coins from rcoins and placing into userid
    //used for signing coins with userid and password

    //get user info to verify coin from yearlydb
    //check for coin in coins.sqlite
    //encrypt coin during validation with user password then return coin address

    QString ekey;
           // int euserid;
    QString password;
    QString datetime;

    //if euserID = "" only check rcoins and coins

    db.setDatabaseName("database.sqlite");
    db.open();
     //   QSqlDatabase::database().transaction();
        QSqlQuery query2;
        query2.exec("SELECT * FROM users WHERE userid = ""'"+euserID.toLatin1()+"'");
        while (query2.next()) {
            // euserid = query.value(0).toInt(); //not encrypted with user password
             ekey = query2.value(0).toString();
            password = query2.value(4).toString();
            datetime = query2.value(6).toString(); //datetime
            qDebug() << euserID.toLatin1() << "pass " << password << "ekey " << ekey;
            //qDebug() << datetime;
          //  return yeardb;
        }
     //   QSqlDatabase::database().commit();
    db.close();


    //use password to decrypt coin then masterkey and check in coinsdb with index

      QString yeardb;
    // coin
    qDebug() <<    datetime.mid(0,4);
    qDebug() <<     datetime.left(4);

//if coin already in yeardb return "valid" else return encrypted coinid
    db.setDatabaseName("./db/"+yeardb.toLatin1()+".sqlite");
    db.open();
     //   QSqlDatabase::database().transaction();
        QSqlQuery query;
        query.exec("SELECT * FROM users WHERE name = " "'" + euserID.toLatin1() + "'" " AND addr = "+coin.toLatin1());
        while (query.next()) {
         //   yeardb = query.value(0).toInt();
            qDebug() << "coin " << query2.value(0).toString();
           // return yeardb.toLatin1();
        }
    //    QSqlDatabase::database().commit();
    db.close();

  //check indexes match in coinsdb and rcoins
//    db.setDatabaseName("coins.sqlite");
//    db.open();
//        QSqlDatabase::database().transaction();
//        QSqlQuery query;
//        query.exec("SELECT * FROM users WHERE name = " "'" + euserID.toLatin1() + "'" " AND addr = "+coin.toLatin1());
//        while (query.next()) {
//         //   yeardb = query.value(0).toInt();
//            qDebug() << "coin " << query2.value(0).toString();
//           // return yeardb.toLatin1();
//        }
//        QSqlDatabase::database().commit();
//    db.close();
//possibly get coin ready to place back into rcoins decrypted
    //return ecoin;
}

QString MainWindow::validateID(QString euserid){ // can validate public encrypted and master encrypted ID's
//also sets QString yeardb globally for other functions


            //search for user in database.sqlite first ? double might be better then use encryption key to find userid in yeardb ?
QString ekey;
       // int euserid;
//QString password;
QString userid;

QString datetime;
vpublickey = 0;

if (euserid.length() <= 12){ //check if encrypted
    qDebug() << "userid" << euserid.toLatin1() ;
   // QString crypted = simplecrypt(ui->lineEditName->text().toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512);
   // euserid = crypted.toLatin1();
qDebug() << "userid now encrypted" << euserid.toLatin1() ;
// QString decrypted = simpledecrypt(euserid.toLatin1(),"password",QCryptographicHash::Sha512);
 //euserid = simpledecrypt(decrypted,masterkey,QCryptographicHash::Sha512);
// euserid = simpledecrypt(euserid,masterkey,QCryptographicHash::Sha512);
           // QString decrypted2="";
}else{
    qDebug() << "userid encrypted" << euserid ;

    // QString decrypted = simpledecrypt(euserid.toLatin1(),masterkey,QCryptographicHash::Sha512);
    euserid = simpledecrypt(euserid,masterkey.toLatin1(),QCryptographicHash::Sha512);
}


qDebug() << "searching valid id" << euserid;
            db.setDatabaseName("database.sqlite");
            db.open();
            //    QSqlDatabase::database().transaction();
                QSqlQuery query2;
              //  query2.exec("SELECT * FROM users WHERE userid = ""'"+euserid.toLatin1()+"'");
                query2.exec("SELECT * FROM users");
                //query2.exec("SELECT "+euserid.toLatin1()+" FROM users");

                while (query2.next()) {
                   //  userid = query2.value(1).toString(); //not encrypted with user password
                    qDebug() << query2.value(1).toString();
                    if (euserid.toLatin1() == simpledecrypt(query2.value(1).toString().toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512)){
                        userid = query2.value(1).toString();
                        qDebug() << "matches" << euserid.toLatin1() ;

                        ekey = query2.value(6).toString();
                      //  userid = query.value(1);
                   //    datetime = query2.value(6).toString()
                       validatepassword = query2.value(3).toString();
                       qDebug() << euserid.toLatin1() << "pass " << validatepassword << "ekey " << ekey;
                    }// else{
                   // qDebug() << 'not proper userid';
                   // }


                  //  return yeardb;
                }
          //      QSqlDatabase::database().commit();
            db.close();


            // if (ui->encrypted_yes->text() == 1){ //extra encryption check
            if (userid == "" && trycount < 10) { //public key check revalidate with decrypted if not found
                      userid = simpledecrypt(euserid,masterkey,QCryptographicHash::Sha512);
                      trycount++;
                      qDebug() << "was public encrypted userid because no match found";
                  //    if ( validateID(userid) == 1 ) { //retry with encrypted
                    //      vpublickey = 1;
                   // }else{
                   // return 0;
                  //  }
            }
QString yeardb2;
            if (vpublickey==1){
                    QString decrypted = simpledecrypt(euserid.toLatin1(),validatepassword,QCryptographicHash::Sha512);
                    userid=decrypted;
            }else{
//                qDebug() << "decrypt to get year";
//                QString decrypted = simpledecrypt(euserid.toLatin1(),masterkey,QCryptographicHash::Sha512);
//                yeardb2 = simpledecrypt(decrypted,masterkey,QCryptographicHash::Sha512);
            }


  //  yeardb = yeardb2;
    // yeardb= yeardb.mid(0,4);
    yeardb = euserid.left(4);
    qDebug() << yeardb;


//verify decrypted id


        db.setDatabaseName("./db/"+yeardb.toLatin1()+".sqlite");
     //   db.open();
        if(db.open())
        {
           qDebug()<<"Successful database connection";
        }
        else
        {
           qDebug()<<"Error: failed database connection";
        }
                qDebug() << "searching yeardb for valid id";
         //   QSqlDatabase::database().transaction();
            QSqlQuery query;
           // query.exec("SELECT * FROM users WHERE userid = " "'" + euserid.toLatin1() + "'");
            query.exec("SELECT * FROM sqlite_master WHERE type='table'");
            while (query.next()) {
               // yeardb =
               // qDebug() << query.value("sql").toString();
             //   if (query.value("type").toString() == "table")
             //      qDebug() << query.value("name");
qDebug() <<query.value(0).toString().toLatin1();
qDebug() <<query.value(1).toString().toLatin1();
                 //qDebug() << query2.value(0).toString().toLatin1() ;
                if (euserid.toLatin1() == simpledecrypt(query.value(1).toString().toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512)){
                     qDebug() << "found user in yeardb" ;
                             return query.value(1).toString().toLatin1();
                }

               // return yeardb.toLatin1();

            }
        //    QSqlDatabase::database().commit();
        db.close();

    return "";
}

float MainWindow::checkBalance(QString euserID,QString yeardb){
//iterate and count coins from userid from yeardb
    float balance=0;

    db.setDatabaseName("./db/"+yeardb.toLatin1()+".sqlite");
    db.open();
     //   QSqlDatabase::database().transaction();
        QSqlQuery query;
      //  query.exec("SELECT * FROM coins WHERE name = ""'"+euserID+"'");
        query.exec("SELECT * FROM ""'"+euserID+"'");
        if (query.next()) {
            balance++;
        }
      //  QSqlDatabase::database().commit();
    db.close();

    balance = balance * ui->coinvalue->text().toFloat();
  return balance;
}


int MainWindow::checkAllCoins(QString db2){
    //check available coins has enough for tx
        int coins=0;
        qDebug() << "checking all available coins";
        db.setDatabaseName(db2.toLatin1());
        db.open();
            QSqlQuery query;
           // query.exec("SELECT * FROM coins");
            query.exec("SELECT * FROM coins");
            while (query.next()) {
             //   int employeeId = query.value(0).toInt();

                qDebug() << query.value(2).toString(); //addr
                qDebug() << query.value(1).toInt(); //origindex
                qDebug() << query.value(0).toInt(); //index

                coins++;
            }
        db.close();

  return coins;
}

int MainWindow::checkAvailableCoins(QString db2,QString needed){
    //check available coins has enough for tx
        int coins=0;
        qDebug() << "checking enough coins available for tx";

        db.setDatabaseName(db2.toLatin1());
        db.open();
            QSqlQuery query;
           // query.exec("SELECT * FROM coins");
            query.exec("SELECT * FROM coins LIMIT "+needed.toLatin1());
            while (query.next()) {
             //   int employeeId = query.value(0).toInt();

                qDebug() << query.value(2).toString(); //addr
                qDebug() << query.value(1).toInt(); //origindex
                qDebug() << query.value(0).toInt(); //index

                coins++;
            }
        db.close();

  return coins;
}


void MainWindow::getkeys(){ //for coldstorage server or standalone server which contains all the infos

    qDebug() << "getting keys";
    //if keys are stored in the local folder and checkout then use those

    //verify md5sum of keys file from 2 or 3 locations possibly encrypted

    //simple strings found on google have same md5sums or bruteforce could match it.

    //load keys if sum is correct
    QFile MyFile(usbpath.toLatin1()+"keys.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;
    QRegExp rx("[:]");
    do {
        line = in.readLine();
        if (line.contains(":")) {
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());

masterkey=nums.at(0);
coinkey=nums.at(1);

}

void MainWindow::on_SendCoins_clicked()
{
   // ListUSB();
   // getkeys();
    //if no keys then generatetx file
    //autosync with perodic checking on a timer for server and client

    md5verifydb();

 if (ui->givecoinsid->text().toLatin1()!=""){
    //could impliment rounding to make ammount proper
    float remainder =  fmod(ui->givecoinsammount->text().toFloat() ,ui->coinvalue->text().toFloat()); // int % int
    if ( remainder > 0 ) // even ammount //check ammount is devisible by value
    {
        qDebug() << remainder;
        qDebug() << "not a proper ammount";

        QMessageBox Msgbox;
            Msgbox.setText("not proper ammount use value ");
            Msgbox.exec();


        return;
    }

     QString result2;


    //check if 2 userid's provided
    if (ui->givecoinsid2->text() == ""){
        //check available coins has enough for tx
        int availableCoins = checkAvailableCoins("rcoins.sqlite", ui->givecoinsammount->text().toLatin1() );

        qDebug() << availableCoins;

        if (availableCoins == ui->givecoinsammount->text().toFloat()){
            qDebug() << "coins are available for tx";
        }            else{
            qDebug() << "not enough for tx";
        }


        //check to see if userid is encrypted otherwise no need to encrypt
      //  QString crypted = simplecrypt( ui->givecoinsid->text().toLatin1(), masterkey.toLatin1(), QCryptographicHash::Sha512);
    //    qDebug() << crypted;
    //    QString decrypted = simpledecrypt(crypted,"test2",QCryptographicHash::Sha512);
    //    qDebug() << decrypted;
trycount=0;
        QString result = validateID(ui->givecoinsid->text().toLatin1()).toLatin1();
        qDebug() << "validate" << result;

        float balance = checkBalance(ui->givecoinsid->text().toLatin1(), result.toLatin1());
        qDebug() << "balance:" << balance;


        //    //find random coin and insert it ammount times
        //generateTXfile(result, etxcoins);
        placeCoins(result,ui->givecoinsammount->text()); // use generated tx


        //    //validate coins have been moved successfully and are valid in coins and id matches place coins into yearly usersdb
            float balance2 = checkBalance(ui->givecoinsid->text().toLatin1(), result.toLatin1());
            float total = ui->givecoinsid->text().toFloat() + balance;

            //removeCoins();
            //for each coin in list
        //    QString::number(balance2-total).toInt() ;

            if( fabs(balance2-total) < 1 ){ //remove coins from rcoins
                    db.setDatabaseName("rcoins.sqlite");
                    db.open();
                   //     QSqlDatabase::database().transaction();
                        QSqlQuery query2;
                        query2.exec("SELECT * FROM employee WHERE name = ''");
                        while (query2.next()) {
        //                    int employeeId = query.value(0).toInt();

        //                    query.exec("INSERT INTO project (id, name, ownerid) "
        //                               "VALUES (201, '', "
        //                               + QString::number(employeeId) + ')');
                        }
                   //     QSqlDatabase::database().commit();
                    db.close();
            } else{
                qDebug() << "coins not removed from rcoins";
                //placeCoins back into rcoins reverse tx
            }



    }else {

        //do this later
     //   QString crypted = simplecrypt( ui->givecoinsid->text().toLatin1(), masterkey.toLatin1(), QCryptographicHash::Sha512);
    //    qDebug() << crypted;
    //    QString decrypted = simpledecrypt(crypted,"test2",QCryptographicHash::Sha512);
    //    qDebug() << decrypted;

     //   result2 = validateID(crypted.toLatin1()).toLatin1(); //returns year
     //   db.setDatabaseName("./"+ result2 +".sqlite");
    }


//    update ammount in database
//    db.setDatabaseName("database.sqlite");
//    db.open();
//        QSqlDatabase::database().transaction();
//        QSqlQuery query2;
//        query.exec("SELECT id FROM users WHERE name = ''");
//        if (query.next()) {
//            int employeeId = query.value(0).toInt();
//            query.exec("UPDATE INTO coins (id, name, ownerid) "
//                       "VALUES (201, '', "
//                       + QString::number(employeeId) + ')');
//        }
//        QSqlDatabase::database().commit();
//    db.close();


//    QMessageBox Msgbox;
//        Msgbox.setText("coins sent ");
//        Msgbox.exec();
}
 //    //find user in yearly db pull coins out and verify validity then place back into rcoins
 //    //re-md5sum file

}

void MainWindow::on_pushButton_2_clicked()
{
    cleartablesusers();
}

void MainWindow::on_randomSearch_clicked()
{//for picking lucky users
    //repurposed temporarly for sqltest
    QString sql = "SELECT * FROM users ORDER BY random()";

    SQLTest("database.sqlite",sql.toLatin1());

}

void MainWindow::on_test_clicked()
{
    QString test = rot13("test");
    qDebug() << test;
    qDebug() << rot13(test.toLatin1());
}

int MainWindow::smtpsend(QString toemail,QString Message){

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
    text.setText("Hi,\nThis is a simple email message.\n");
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
}

void MainWindow::on_smtpsave_clicked()
{

}

void MainWindow::on_smtptestmessage_clicked()
{

}

