#ifndef VALIDATECOINS_H
#define VALIDATECOINS_H
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include  <math.h>
//#include <QCoreApplication>
#include <QFile>
#include <QDebug>
//#include <QMainWindow>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>



QString MainWindow::decodetxQR(){

qDebug() << "test";

}

void MainWindow::on_generatetx_clicked()
{
    //GenerateQRCode()
    QString result = generateRXfile(mainID.toLatin1(),ui->receiveid->text().toLatin1(),ui->receiveammount->text().toLatin1());
    //generateTXfile()
}



void MainWindow::on_GenerateRequest_clicked()
{
    //could be sent via smtp

    if ( ui->receiveid->text().toLatin1() == "" ){
        //generate purchase from server request

    }else{
    //if admin then placecoins without generating rxfile
        //verify keys
    placeCoins( ui->receiveid->text().toLatin1(), ui->receiveammount->text().toLatin1());

    //else
    //    QString result = generateRXfile(mainID.toLatin1(),ui->receiveid->text().toLatin1(),ui->receiveammount->text().toLatin1());

    }
    //QString requeststring= ;
 //   generateRXfile();
//GenerateQRCode(requeststring.toLatin1())
}



void MainWindow::on_validatecoins_clicked()
{
// in order to validate all coins we need to first start with the databases then rcoins and compare to coins.sqlite to see if all are accounted for.
}


int MainWindow::processRXTXfile(QString file){

    //check if client or admin
    //if client just add rx into wallet
    //server must process tx files to validate

//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.rx *.tx *.txt)"));
//    qDebug()<< fileName.toLatin1() ;

    QFile MyFile(file.toLatin1());
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

    nums.at(0);  // sender
    nums.at(1);  // receiver
    nums.at(2); // ammount
    nums.at(4); // md5sum

    //walletCoinInsert(QString ID,QString CoinAddress,QString Owner,QString cid,QString date)
}


QString MainWindow::generateTXfile(QString suserid,QString ruserid,QString etxcoins){ //file to send from client
    //encrypt with masterkey encrypted userID and user encryption key and datetime to validate coins from their wallet

    //might only need ammount and userid and other usersid for tx's because users are not sending coins directly to other users
    QString fileName2 = QFileDialog::getSaveFileName(this,  tr("Save TX"), "",  tr("SaveRX/TX File (*.txt);;All Files (*)"));

 //   db.setDatabaseName("./"+ result +".sqlite");
//       db.open();
//           QSqlDatabase::database().transaction();
//           QSqlQuery query;
//         //  query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
//          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
//           while (query.next()) {
//               int employeeId = query.value(0).toInt();
//           }
//           QSqlDatabase::database().commit();
//       db.close();

    QFile file(fileName2);
       if(file.open(QIODevice::ReadWrite | QIODevice::Text))
       {
           QTextStream stream(&file);

        file.close();
       }

       //append md5sum

}

QString MainWindow::generateRXfile(QString ruserid,QString suserid,QString etxcoins){ //rxfile to give client encrypted coins to put in wallet might not be needed. for client only cold server

    //etxcoins is either tmptxtfile with encryptedcoins or to make more secure use memory
    //encrypt coin addresses with mastercoinkey and userid to send the coins to their wallets if so desired (upto the user for extra security also gives ability to send between user accounts)
    //verify with time encrypted password verify and send time info password encrypted with userid
    // do they get their actual userid or an encrypted version based on masterkey and their password

    QString fileName2 = QFileDialog::getSaveFileName(this,  tr("Save TX"), "",  tr("SaveRX/TX File (*.txt);;All Files (*)"));


//       db.setDatabaseName("./"+ result +".sqlite");
//       db.open();
//           db.transaction();
//           QSqlQuery query;
//         //  query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
//          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
//           while (query.next()) {
//               int employeeId = query.value(0).toInt();
//             //  rcoins <<      //decrypt coins and reencrypt for new user
//               //can place into text file to be sure then delete here// verify enough is available
//           }
//           db.commit();
//       db.close();

    QFile file(fileName2);
       if(file.open(QIODevice::ReadWrite | QIODevice::Text))
       {
           QTextStream stream(&file);

        file.close();
       }
       //append md5sum


}

QString MainWindow::validateCOINsign(QString coin,QString euserID){ // for getting coins from rcoins and placing into userid
    //used for signing coins with userid and password and index,datetime
    //encrypt coin during validation with user password then return coin address

    qDebug() << "looking for coin" << coin.toLatin1();
    QString ekey;
           // int euserid;
    QString password;
    QString datetime;

    // check if validating signed or unsigned coin
    if (euserID.toLatin1() == ""){ // only check rcoins and coins for unencrypted coins
        //check indexes match in coinsdb and rcoins


          db.setDatabaseName("coins.sqlite");
          db.open();
          db.transaction();
              QSqlQuery query4;
              query4.exec("SELECT * FROM users WHERE name = " "'" + euserID.toLatin1() + "'" " AND addr = "+coin.toLatin1());
              while (query4.next()) {
               //   yeardb = query.value(0).toInt();
                  qDebug() << "coin " << query4.value(0).toString();
                 // return yeardb.toLatin1();
              }
          db.commit();
          db.close();



          db.setDatabaseName("rcoins.sqlite");
          db.open();
          QSqlDatabase::database().transaction();
              QSqlQuery query5;
              query5.exec("SELECT * FROM coins WHERE addr=""'"+coin.toLatin1()+"'");
              while (query5.next()) {
               //   yeardb = query.value(0).toInt();
                  qDebug() << "coin " << query5.value(0).toString();
                  return  "valid";
              }
          QSqlDatabase::database().commit();
          db.close();


    }else{        ///check user exists and get signing info to sign coins with
qDebug() << "signing coin then getting another";
        if (coin.length() > 8 ){ //if encrypted unsign coin
            qDebug() << "encrypted coin during verify unsingning";
            db.setDatabaseName("wallet.sqlite"); //search for signed coin in db then unsign for placement and resigning
            db.open();
                QSqlDatabase::database().transaction();
                QSqlQuery query2;
                query2.exec("SELECT * FROM users WHERE coin = ""'"+euserID.toLatin1()+"'");
                while (query2.next()) {
                     //userid = query.value(1).; //not encrypted with user password
                     ekey = query2.value(7).toString();
                    password = query2.value(4).toString();
                    datetime = query2.value(6).toString(); //datetime
                    qDebug() << "user " << euserID.toLatin1() << " pass " << password << "ekey " << ekey;
                    //qDebug() << datetime;
                  //  return yeardb;
                }
                QSqlDatabase::database().commit();
            db.close();

         //   signedcoin = simplecrypt(ecoinuser,ekey.toLatin1(),QCryptographicHash::Sha512);
         //   QString ecoinuser =  simplecrypt(euserID.toLatin1(),ecoin.toLatin1(),QCryptographicHash::Sha512);
         //   QString ecoin =  simplecrypt(signedcoin.toLatin1(),coinkey.toLatin1(),QCryptographicHash::Sha512);
        }

    db.setDatabaseName("database.sqlite");
    db.open();
        QSqlDatabase::database().transaction();
        QSqlQuery query2;
        query2.exec("SELECT * FROM users WHERE userid = ""'"+euserID.toLatin1()+"'");
        while (query2.next()) {
             //userid = query.value(1).; //not encrypted with user password
            ekey = query2.value(7).toString();
            password = query2.value(4).toString();
            datetime = query2.value(6).toString(); //datetime
            qDebug() << "user " << euserID.toLatin1() << " pass " << password << "ekey " << ekey;
            //qDebug() << datetime;
          //  return yeardb;
        }
        QSqlDatabase::database().commit();
    db.close();


    //use password to decrypt coin then masterkey and check in coinsdb with index

    //  QString yeardb;
    // coin
  //  qDebug() <<    datetime.mid(0,4);
  //  qDebug() <<     datetime.left(4);
//int missingcoin=0;
    qDebug() << "verify coin in rcoins";
    db.setDatabaseName("rcoins.sqlite");
    db.open();
    QSqlDatabase::database().transaction();
        QSqlQuery query3;
        query3.exec("SELECT addr FROM coins WHERE trim(addr) =" "'" + coin.toLatin1() + "'"); // match and contains // collate binary  trim(addr)
      //  query3.exec("SELECT addr FROM coins WHERE addr like " "'" + coin.toLatin1() + "'""ORDER BY random()");
        while (query3.next()) {
           qDebug() << "tcoin " << query3.value(0).toString();
           qDebug() << "tcoin " << coin.toLatin1();
           if ( query3.value(0).toString().toLatin1() ==  coin.toLatin1() ){
                //yeardb = query.value(0).toInt();
                qDebug() << "coin " << query3.value(0).toString();
                //sign coin with info
                //password ekey datetime index;
                QString signedcoin=query3.value(0).toString(); //if not signing needs value
               // return query3.value(0).toString();
                if (1){
                    qDebug() << "coin: " << signedcoin.toLatin1();
                    qDebug() << "coinkey coin: " << coinkey.toLatin1();
               QString ecoin =  simplecrypt(signedcoin.toLatin1(),coinkey.toLatin1(),QCryptographicHash::Sha512);
               qDebug() << "signedcoin: " << signedcoin.toLatin1();
               qDebug() << "userID coin: " << coinkey.toLatin1();
               QString ecoinuser =  simplecrypt(euserID.toLatin1(),ecoin.toLatin1(),QCryptographicHash::Sha512);
               qDebug() << "usersigned coin: " << signedcoin.toLatin1();
               signedcoin = simplecrypt(ecoinuser,ekey.toLatin1(),QCryptographicHash::Sha512);
               }
                qDebug() << "final usersigned coin: " << signedcoin.toLatin1();
                       db.close();
            return signedcoin;
           }
        }
    QSqlDatabase::database().commit();
    db.close();
    query3.clear();
  qDebug() << "no valid coin";
  return "";

//  if (missingcoin ==1 ){
//      }
//  }
//if coin already in yeardb return "valid" else return encrypted coinid
//    db.setDatabaseName("./db/"+yeardb.toLatin1()+".sqlite");
//    db.open();
//     //   QSqlDatabase::database().transaction();
//        QSqlQuery query6;
//        query6.exec("SELECT * FROM users WHERE name = " "'" + euserID.toLatin1() + "'" " AND addr = ""'"+coin.toLatin1()+"'");
//        while (query6.next()) {
//         //   yeardb = query6.value(0).toInt();
//            qDebug() << "coin " << query6.value(0).toString();
//           // return "valid";
//        }
//    //    QSqlDatabase::database().commit();
//    db.close();

    //sign coins

}

//possibly get coin ready to place back into rcoins decrypted
    //return ecoin;
}


QString MainWindow::validateID(QString userid){ // can validate public encrypted and master encrypted ID's
//also sets QString yeardb globally for other functions

QString ekey;
QString euserid;
QString datetime;

vpublickey = 0;
//match the public ekey to get the userid
if (userid.length() <= 12){ //check if encrypted
    qDebug() << "userid" << userid.toLatin1() ;
}else{
    qDebug() << "userid encrypted" << userid ;
    euserid = simpledecrypt(userid,masterkey.toLatin1(),QCryptographicHash::Sha512);
}

qDebug() << "searching valid id" << userid;
            db.setDatabaseName("database.sqlite");
            db.open();
                QSqlDatabase::database().transaction();
                QSqlQuery query2;
              //  query2.exec("SELECT * FROM users WHERE userid = ""'"+euserid.toLatin1()+"'");
                query2.exec("SELECT * FROM users");
                //query2.exec("SELECT "+euserid.toLatin1()+" FROM users");

                while (query2.next()) {
                   //  userid = query2.value(1).toString(); //not encrypted with user password
                    qDebug() << query2.value(1).toString();

                    validatepassword = query2.value(3).toString();

                    if (userid.toLatin1() == query2.value(7).toString().toLatin1()){ // check if userid matches ekey or public key
                        userid = simpledecrypt(userid,ekey.toLatin1(),QCryptographicHash::Sha512);
                    }

                    if (userid.toLatin1() == simpledecrypt(query2.value(1).toString().toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512)){
                        euserid = query2.value(1).toString();
                        qDebug() << "matches" << euserid.toLatin1() ;
                        ekey = query2.value(7).toString();
                      //userid = query.value(1);
                      //datetime = query2.value(6).toString()
                        qDebug() << userid.toLatin1() << "pass " << validatepassword << "ekey " << ekey;
                    } else{
                    qDebug() << 'not proper userid';
                    }
                  //  return yeardb;
                }
            QSqlDatabase::database().commit();
            db.close();

    // yeardb= yeardb.mid(0,4);
    yeardb = userid.left(4);
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
            QSqlDatabase::database().transaction();
            QSqlQuery query;
           // query.exec("SELECT * FROM users WHERE userid = " "'" + euserid.toLatin1() + "'");
            query.exec("SELECT * FROM sqlite_master WHERE type='table'");
            while (query.next()) {
               // qDebug() << query.value("sql").toString();
             //   if (query.value("type").toString() == "table")
             //   qDebug() << query.value("name");
                qDebug() <<query.value(0).toString().toLatin1();
                qDebug() <<query.value(1).toString().toLatin1();
                 //qDebug() << query2.value(0).toString().toLatin1() ;
                if (userid.toLatin1() == simpledecrypt(query.value(1).toString().toLatin1(),masterkey.toLatin1(),QCryptographicHash::Sha512)){
                     qDebug() << "found user in yeardb" ;
                             return query.value(1).toString().toLatin1();
                }
               // return yeardb.toLatin1();
            }
            QSqlDatabase::database().commit();
        db.close();

    return "";
}



#endif // VALIDATECOINS_H
