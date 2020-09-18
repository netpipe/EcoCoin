#ifndef WALLET_H
#define WALLET_H

#include <mainwindow.h>
#include "ui_mainwindow.h"
//#include <QCoreApplication>
#include <QFile>
#include <QDebug>
//#include <QMainWindow>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QClipboard>
#include <QByteArray>
#include <src/encryption/encryption.h>
#include <QDirIterator>

#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/JlCompress.h"

int MainWindow::createPickupCoinTable(QString eownerID){

    db.setDatabaseName("./db/pickup.sqlite");

    if(db.open())    {       qDebug()<<"Successful database connection";    }
    else    {       qDebug()<<"Error: failed database connection";    }

    QString query;

    qDebug() << "test" << eownerID.toLatin1();

    query.append("CREATE TABLE IF NOT EXISTS ""'"+eownerID.toLatin1()+"'""("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "origindex VARCHAR(100)," //rcoins index then coins.sqlite is stored on usbdrive as part of key/verify
                    "addr VARCHAR(100),"
                    "datetime INTEGER,"
                    "class INTEGER,"
                    "hold INTEGER"
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

int MainWindow::pickupCoinsInsert(QString eownerID,QString Coin){
//createPickupCoinTable();

}



void MainWindow::serverusbtxrx(){
    //automatic function to do rxtx from usb for cold storage

    //verify tx file apply
    QStringList list;
    //export db's and overwrite if valid
    QDirIterator it("./db/", QStringList() << "*.sqlite", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
      //  QFileInfo fileInfo(f.fileName());
     list << it.next().toLatin1();
    }

#ifdef QUAZIP
    if(JlCompress::compressFiles("saveFile.zip", list)){
//        QMessageBox Msgbox;
//            Msgbox.setText("zipped");
//            Msgbox.exec();
    } else {
                   QMessageBox Msgbox;
                       Msgbox.setText("zip file not found ");
                       Msgbox.exec();
    }
#endif

}

void MainWindow::clientusbtxrx(){
    //import db's and overwrite if valid md5sums after copying yearly dbs and md5sums from server
    //applying rx file to compare?

    QStringList list;
    //export db's and overwrite if valid
    QDirIterator it("./db/", QStringList() << "*.sqlite", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
      //  QFileInfo fileInfo(f.fileName());
     list << it.next().toLatin1();
    }

#ifdef QUAZIP
    if(JlCompress::compressFiles("saveFile.zip", list)){
//        QMessageBox Msgbox;
//            Msgbox.setText("zipped");
//            Msgbox.exec();
    } else {
                   QMessageBox Msgbox;
                       Msgbox.setText("zip file not found ");
                       Msgbox.exec();
    }


    unCompress("saveFile.zip" , "./db/");
#endif
}



void MainWindow::on_randomSearch_clicked()
{//for picking lucky users
    //repurposed temporarly for sqltest
//    QString sql = "SELECT * FROM users ORDER BY random()";

//    SQLTest("database.sqlite",sql.toLatin1());

    selectUsers();

}



void MainWindow::on_gencoininfo_btn_clicked()
{
        qDebug() << "generating settings.txt";
    QFile file("settings.txt");
      //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          if(file.open(QIODevice::ReadWrite | QIODevice::Text))
          {
              QTextStream stream(&file);
              stream << "CoinName:" << ui->coinname->text() <<'\n';
              stream << "CoinCount:" << ui->coincount->text() <<'\n';
              stream << "CoinLength:" << ui->coinlength->text() <<'\n';
              stream << "createday:" << ui->createday->text() <<'\n';
              stream << "CreateMonth: " << ui->createmonth->currentText() <<'\n';
              stream << "createyear:" << ui->createyear->text() <<'\n';
              stream << "createtime:" << ui->createtime->time().hour() << "|" << ui->createtime->time().minute()  <<'\n';
              stream << "coinvalue:" << ui->coinvalue->text() <<'\n';
              stream << "matures:" << ui->matureradio_yes->text() <<'\n';
              stream << "coinpayout:" << ui->coinpayout->text() <<'\n';
              stream << "encrypted:" << ui->encrypted_yes->text() <<'\n';
              stream << "maturedate:" << ui->matureday->text() <<'\n';
              stream << "maturemonth:" << ui->maturemonth->currentText() <<'\n';
              stream << "matureyear:" << ui->matureyear->text() <<'\n';
              stream << "maturetime:" << ui->maturetime->time().hour() << "|" << ui->maturetime->time().minute() <<'\n';
              stream << "maturedescription:" << ui->maturedescription->toPlainText()<<'\n';
              stream << "usbdrivename:" << ui->usbdrivename->text() <<'\n' ; //place into keys instead
              file.close();
          }

          ListUSB();

        qDebug() << "usb path " << usbpath.toLatin1();
          QString path;
          qDebug() << "usb path set.";
            if(usbpath.toLatin1() != "" ){
            path =     usbpath.toLatin1()+"/keys.txt";
            }else{
                qDebug() << "local keys path set.";
            path = "./keys.txt";
            }
          QFile file2(path);
            //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))

                if(file2.open(QIODevice::ReadWrite | QIODevice::Text))
                {
                    QTextStream stream(&file2);
                    stream << "masterkey:" << masterkey.toLatin1() <<'\n';
                    stream << "coinkey:" << coinkey.toLatin1() <<'\n';
                    stream << "coinsmd5:" << fileChecksum("coins.sqlite",QCryptographicHash::Md5).toHex() <<'\n';
                    stream << "checksum:" << md5Checksum("masterkey:"+ masterkey.toLatin1()+"coinkey:" +coinkey.toLatin1()+"coinsmd5:" + fileChecksum("coins.sqlite",QCryptographicHash::Md5)).toHex() <<'\n';
                  //  stream << "usbdrivename:" << ui->usbdrivename->text();
                    file2.close();
                }
}

void MainWindow::on_pushButtonInsertUser_clicked()
{
    //QString temp = GenerateClientAddress(8);
    QString temp = GetRandomString(8,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
    ui->lineEditName->setText(year.toLatin1()+temp.toLatin1());
    //ui->lineEditName->setText(temp.toLatin1()); //testing
    QString ownerid=ui->lineEditName->text().toLatin1();
    QString password=ui->lineEditPassword->text();

    //    QString s = QDate::currentDate().toString();
    //    QDate::currentDate().day();
    //    QDate::currentDate().month();
    //    QDate::currentDate().year();
    QTime starttime(QTime::currentTime().hour(),QTime::currentTime().minute());
    QDate dNow(QDate::currentDate());
    ui->createuserdatetime->setText(dNow.toString("dd.MM.yyyy")+"T"+starttime.toString());

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

    qDebug() << ownerid.toLatin1() ;

    QString crypted2 = simplecrypt(ownerid.toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512);
   // QString decrypted = simpledecrypt(crypted,"test2",QCryptographicHash::Sha512);
    qDebug() << crypted2 ;
    ui->lineEditName->setText(crypted2.toLatin1());

    createyearly(crypted2);

  //  createyearly(crypted2); //the /n causes issues
  //  qDebug() << "lineeditname " << ui->lineEditName->text();

    insertUser();

    //selectUsersCoins(temp.toLatin1(),year.toLatin1());

    ui->lineEditName->setText(ownerid.toLatin1());
    ui->lineEditName->setEnabled(1);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lineEditName->text());

    on_usergenerateQr_clicked();

    selectUsers(); //refresh user table

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
                    "origindex VARCHAR(100)," //rcoins index then coins.sqlite is stored on usbdrive as part of key/verify
                    "addr VARCHAR(100),"
                    "datetime INTEGER,"
                    "class INTEGER,"
                    "hold INTEGER"
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

void MainWindow::writeAdminFrontendHashes() // for startup and shutdown could be password protected
{
//generate md5sum
QByteArray coinstxtmd5 =  fileChecksum("coins.txt",QCryptographicHash::Md5);
QByteArray coindb =  fileChecksum("coins.sqlite",QCryptographicHash::Md5);
QByteArray availablecoins =  fileChecksum("availableCoins.sqlite",QCryptographicHash::Md5);

QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
// codec->toUnicode(coindb)

QFile hashfile("hashes.txt");
if(hashfile.open(QIODevice::WriteOnly | QIODevice::Text))
{
    QTextStream stream(&hashfile);
        //hashfile.seek(0);
        stream << "coinstxt:" << coinstxtmd5.toHex() << endl;
        stream << "coinsdb:" << coindb.toHex() << endl;
        stream << "availableCoins:" << availablecoins.toHex() << endl;
   }
hashfile.close();

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

//      QDir dir("./db/");
//      dir.removeRecursively();

      QFile::remove("coins.txt");
      QFile::remove("database.sqlite");
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
                    "email VARCHAR(100),"
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
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(10).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,4,new QTableWidgetItem(select.value(6).toByteArray().constData()));
        row++;
    }

    query.clear();
    db.close();
}


void MainWindow::on_userssearch_clicked() //search button
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
#ifdef ENCRYPTION
            QByteArray bFname = EncryptMsg(ui->userid->text(),"123456789", "your-IV-vector");
            QString mykey1 = BigInt2Str(m_e); //rsa keys
            QString mykey2 = BigInt2Str(m_n); //rsa keys

            query.append("SELECT * FROM users WHERE name =" "'" + bFname  + "'" );
#endif
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
#ifdef ENCRYPTION
    QString mykey1 = BigInt2Str(m_e); //rsa keys
    QString mykey2 = BigInt2Str(m_n); //rsa keys
#endif
    if (ui->encrypted_yes->text() == "Yes" ){

        while (select.next())
        {
#ifdef ENCRYPTION
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
            #endif
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


void MainWindow::SQLTest(QString dbname,QString Query)
{
    db.setDatabaseName(dbname.toLatin1());
    if(db.open())
    {       qDebug()<<"Successful database connection";    }
    else
    {       qDebug()<<"Error: failed database connection";    }

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

void MainWindow::on_addUserQR_clicked()
{
    QString fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
        if (!fileName.isNull())
        {
            QPixmap pixMap = this->ui->usergenerateQr->grab();
            pixMap.save(fileName);
        }
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

    //could also use md5sum at time of creation for public key + time
    QString crypted2 = simplecrypt(ui->lineEditName->text(),masterkey.toLatin1(),QCryptographicHash::Sha512);

    QString query;

     // QString euserid = ui->lineEditName->text();
#ifdef ENCRYPTION
    QByteArray bFname = EncryptMsg(ui->lineEditName->text(),"123456789", "your-IV-vector");
    QString mykey1 = BigInt2Str(m_e); //rsa keys
    QString mykey2 = BigInt2Str(m_n); //rsa keys
#endif
      //generate public key and encrypt userid

    query.append("INSERT INTO users("
                 "email,"
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
                 "'"+ui->adduserEmail->text().toLatin1()+"',"
                 "'"+crypted2.toLatin1()+"',"
                 "'"+ui->addusername->text().toLatin1()+"',"
                 "'"+ui->lineEditPassword->text().toLatin1()+"',"
                 "'"+ui->lineEditPhone->text().toLatin1()+"',"
                 "'"+ui->createuserdatetime->text()+"',"
                 "'"+ GetReallyRandomString(8, "").toLatin1() +"'," //ekey
                 "'""'," //ammount
                 "'"+ui->createextra->text().toLatin1()+"',"
                 "'"+ui->createclass->text()+"'"
                 ");");

  //  qDebug()<< euserid.toLatin1();

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




#endif // WALLET_H
