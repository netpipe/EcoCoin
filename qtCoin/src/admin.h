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

void MainWindow::on_randomSearch_clicked()
{//for picking lucky users
    //repurposed temporarly for sqltest
    QString sql = "SELECT * FROM users ORDER BY random()";

    SQLTest("database.sqlite",sql.toLatin1());

}


int MainWindow::getkeys(){ //for coldstorage server or standalone server which contains all the infos

    qDebug() << "getting keys";
    //if keys are stored in the local folder and checkout then use those

    //verify md5sum of keys file from 2 or 3 locations possibly encrypted
    //simple strings found on google have same md5sums or bruteforce could match it.
    bool keyexists=0;
    QString path;
    if (ui->usbdrivename->text().toLatin1() != ""){
        ListUSB();
        QString path2;
        path2 = usbpath.toLatin1()+"/keys.txt";
        QFile MyFile2(path2);
        if ( MyFile2.exists() ){        keyexists= true;        path = usbpath.toLatin1()+"/keys.txt";    }
    } else {
        QString path3;
        path3 = "./keys.txt";
        QFile MyFile3(path3);
        if ( MyFile3.exists() ){    keyexists= true;   path = "./keys.txt"; }
    }

    QFile MyFile(path);

    if(MyFile.exists() && keyexists ){
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
    qDebug() << "masterkey" << masterkey;
    coinkey=nums.at(1);
    qDebug() << "coinkey" << coinkey;
    return 1;

    }else {
        return 0;
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


void MainWindow::serverusbtxrx(){
    //automatic function to do rxtx from usb for cold storage
    //export db's and overwrite if valid

}

void MainWindow::clientusbtxrx(){
    //import db's and overwrite if valid

}

void MainWindow::ListUSB(){
    #ifdef STORAGE
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
        //   qDebug() << "yep" << "/n";
            usbpath = storage.rootPath();

            if (storage.isReadOnly())
               qDebug() << "isReadOnly:" << storage.isReadOnly();

//                qDebug() << "name:" << storage.name();
//                qDebug() << "fileSystemType:" << storage.fileSystemType();
//                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
        }
     }

    if (usbpath.toLatin1() == "")
    {
        QMessageBox Msgbox;
            Msgbox.setText("drive not found: ");
            Msgbox.exec();
    }
#endif
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




#endif // WALLET_H
