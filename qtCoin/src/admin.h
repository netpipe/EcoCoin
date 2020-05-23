#ifndef WALLET_H
#define WALLET_H
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include  <math.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
//#include <QMainWindow>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QClipboard>
#include "QRCode/QrCode.hpp"


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
