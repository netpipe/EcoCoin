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
//#include <QTextCodec>
#include <QFileDialog>
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/JlCompress.h"
//#include <QProcess>
#include <QDirIterator>

// for nonencrypted mode only xor the id against coin or append

QString MainWindow::decodetxQR(){
#ifdef BARCODE
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.png *.jpg)"));
    return decodeqr(fileName);

    qDebug() << "testqrdecode";
#endif
}


void MainWindow::on_generatetx_clicked()
{
    //for user offline transactions the coins will be placed into a pickup database on the server
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
        if (admin){    //if admin then placecoins without generating rxfile
            placeCoins( ui->receiveid->text().toLatin1(), ui->receiveammount->text().toLatin1());


        } else {
            QString result = generateRXfile(mainID.toLatin1(),ui->receiveid->text().toLatin1(),ui->receiveammount->text().toLatin1());



        }
    }

    //QString requeststring= ;
    //   generateRXfile();
    //GenerateQRCode(requeststring.toLatin1())
}



void MainWindow::on_validatecoins_clicked() // check to see if they exist. db validate after creation
{
    if (admin){
        // in order to validate all coins we need to first start with the databases then rcoins and compare to coins.sqlite to see if all are accounted for.
        // verify coins.db to availableCoins.sqlite
        // verify availableCoins to coins.txt for now otherwise use coinsdb

        QStringList coins;
        QStringList index;

        //check if file exists
        QFile MyFile("coins.txt");
        MyFile.open(QIODevice::ReadWrite);
        QTextStream in (&MyFile);
        QString line;
        QStringList list;
        QStringList nums;
        QRegExp rx("[:]");
        do {
            line = in.readLine();
            if (line.contains(":")) {
                list = line.split(rx);
                index.append(list.at(0).toLatin1());
                coins.append(list.at(1).toLatin1());

            }
        } while (!line.isNull());


        //        db.setDatabaseName("./available.sqlite");
        //           db.open();
        //               QSqlDatabase::database().transaction();
        //               QSqlQuery query;
        //               query.exec("SELECT * FROM " "'" +suserid.toLatin1() +"'" "LIMIT " "'"+ etxcoins.toLatin1() +"'");
        //              // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
        //              // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
        //               while (query.next()) {
        //                   coins << query.value(0).toString();
        //               }
        //               QSqlDatabase::database().commit();
        //           db.close();

        // verify rcoins against coins.sqlite




    } else {
        //check wallet coins are verified
        //download pickup.sqlite and verify against current coin holdings
        // Download("ftp://admin:qt@127.0.0.1:8285/pickup.sqlite"); //pickup db uses transaction password
    }
}


int MainWindow::processRXTXfile(QString file){

    //check if client or admin
    //if client just add rx into wallet
    //server must process tx files to validate
    //store in pickup on server for offline txs
    //ftp mode and smtp mode , ssl available later
    //

    //    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.rx *.tx *.txt)"));
    //    qDebug()<< fileName.toLatin1() ;

    //if admin mode validate id

    //decrypt file possibly or just coins
    QVariantList coins;
    QVariantList origindex;

    QFile MyFile(file.toLatin1());
    //MyFile.open(QIODevice::ReadWrite);
    if (!MyFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // Handle the error if the file cannot be opened.
        qDebug() << "Failed to open the file for reading and writing:" << MyFile.errorString();
        //return;
    }
    QTextStream in (&MyFile);
    QString line;
    QStringList list;
    QStringList nums;
    QRegExp rx("[:]");
    QRegExp rx2("[->]");
    //int lines;
    do {
        line = in.readLine();
        qDebug() << line;
        if (line.contains(":")) {
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
        else if(line.contains("->")){
            list = line.split(rx2);
            qDebug() << list;
            coins.append(list.at(2).toLatin1());
            origindex.append(list.at(0).toInt());
            qDebug() << list.at(0).toLatin1() << list.at(2).toLatin1();

        }
        // lines++;
    } while (!line.isNull());
    qDebug() << list;
    qDebug() << nums;
    MyFile.close();

    db.setDatabaseName("./db/"+ nums.at(1).toLatin1() +".sqlite");
    if(db.open())    {  qDebug()<<"Successful coin database connection";    }    else    {   qDebug()<<"Error: failed database connection";    }
    // Check and create the 'coins' table if it doesn't exist
    QSqlQuery checkTable;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS coins ("
                               "origindex INTEGER PRIMARY KEY, "
                               "addr TEXT, "
                               "datetime TEXT, "
                               "class INTEGER, "
                               "hold INTEGER)";

    if(checkTable.exec(createTableQuery)) {
        qDebug() << "The 'coins' table is verified or created successfully";
    } else {
        qDebug() << "ERROR! Could not verify or create the 'coins' table:" << checkTable.lastError();
        // Handle error, maybe return or exit
    }
    db.transaction();
    QString query2 = "INSERT INTO 'coins'(origindex,addr,datetime,class,hold) VALUES (?,?,?,?,?)";
    QSqlQuery insert;
    insert.prepare(query2);
    for(int i = 0; i < coins.length(); ++i) {
        // Bind values for each coin
        insert.bindValue(0, origindex[i]);
        insert.bindValue(1, coins[i]);
        // Bind additional values for datetime, class, and hold as needed
        insert.bindValue(2, 1);
        insert.bindValue(3, 0);
        insert.bindValue(4, 0);

        if(!insert.exec()) {
            qDebug() << "Error inserting data: " << insert.lastError();
            db.rollback();  // Rollback transaction on error
            break;  // Exit the loop if there is an error
        }
    }


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
    insert.clear();

    db.close();

    //QFile MyFiletx("tmp.txt");
    //MyFiletx.open(QIODevice::ReadWrite);
    //QTextStream intx (&MyFiletx);
    //save to tmp file and md5sum
    //intx << nums.at(0);  // sender
    //intx << nums.at(1);  // receiver
    //intx << nums.at(2); // ammount
    //intx << nums.at(3); //datetime
    //int i=4;
    //foreach (QString tmp , nums){
    //         i++;
    //         intx << nums.at(i).toLatin1();
    // coins <<   nums.at(i).toLatin1();  // unencrypt and reencrypt to tx password

    //}

    //coins
    // nums.at(nums); // md5sum  load all but last line of file then verify file md5sum
    //coins get put into recoinsdb with nums.at(1);  // receiver ID
    //MyFiletx.close();

    //validateCOINsign();

    //client will be able to establish trust by providing a decrypt string

    if (admin){
        //encrypted public list of signed coins yearly db's can be provided for offline verify via ftp

    } else {
        //send to server and get balance back or verify with pickupdb

        //validateCOINsignWallet();
        //walletCoinInsert(QString ID,QString CoinAddress,QString Owner,QString cid,QString date)

    }



    return 0;
}


QString MainWindow::generateTXfile(QString suserid,QString ruserid,QString etxcoins){ //file to send from client

    //for offline transactions it can send multiple times with random delays and store them in pickup db
    //encrypt with masterkey encrypted userID and user encryption key and datetime to validate coins from their wallet

    //might only need ammount and userid and other usersid for tx's because users are not sending coins directly to other users
    QString fileName2;
    QVariantList coins;
    QVariantList origindex;

    if (admin){ // maybe check if frontend or backend
        fileName2="./rxtx/tmp.txt";



    }else {
        fileName2 = QFileDialog::getSaveFileName(this,  tr("Save TX"), "",  tr("SaveRX/TX File (*.txt);;All Files (*)"));
        //fileName2 = QFileDialog::getSaveFileName(this,  tr("Save TX"), "",  tr("SaveRX/TX File (*.txt);;All Files (*)"));
        qDebug() << "doc generate:";
        QFile file(fileName2);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            // Handle the error if the file cannot be opened.
            qDebug() << "Failed to open the file for reading and writing:" << file.errorString();
            //return;
        }

        QTextStream stream(&file);

        // Writing data to the stream.
        stream << "sender:" << suserid.toLatin1() << "\n";
        stream << "receiver:" << ruserid.toLatin1() << "\n";
        stream << "amount:" << etxcoins << "\n";

        QDateTime currentDateTime = QDateTime::currentDateTime();
        stream << "datetime:" << currentDateTime.toString("dd.MM.yyyy'T'HH:mm:ss") << "\n";


        file.close();
        // Reopen the file in append mode to add the checksum.
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Could not open file for appending:" << file.errorString();
            //return;
        }
        // Calculate and write the MD5 checksum of the file.
        //file.seek(0); // Go to the beginning of the file to read its contents for the checksum.
        QString md5Checksum = fileChecksum(file.fileName(), QCryptographicHash::Md5);
        stream << "md5:" << md5Checksum << "\n";
        db.setDatabaseName("./db/"+ suserid.toLatin1() +".sqlite");
        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query;
        query.exec("SELECT * FROM coins ORDER BY random() LIMIT "+etxcoins.toLatin1());
        while (query.next()) {
            coins << query.value(1).toString();
            origindex << query.value(0).toString();
            qDebug() << "picked coins" << query.value(1).toString();
            stream << query.value(0).toString()+"->" << query.value(1).toString() << "\n";
        }
        QSqlDatabase::database().commit();
        origindex.clear();
        db.close();

        qDebug() << "Document saved successfully.";
        file.close();

        //return 0;
        qDebug() << "doc generate:";
        db.setDatabaseName("./db/"+ suserid.toLatin1() +".sqlite");
        //db.open();
        if(db.open())    {  qDebug()<<"Successful rcoin database connection";    }    else    {   qDebug()<<"Error: failed database connection";    }
        QSqlDatabase::database().transaction();
        QSqlQuery query3;
        for (int i=0; i < etxcoins.toInt(); i++ ) {
            qDebug() << coins.at(i).toString().toLatin1();
            query3.exec("SELECT * FROM coins WHERE addr LIKE " "'" + coins.at(i).toString().toLatin1() + "'" "ORDER BY random()");
            while (query3.next()) {
                qDebug() << "rcoin " << query3.value(1).toString();
                qDebug() << "rcoin2 " << coins.at(i).toString().toLatin1();
                if ( query3.value(1).toString().toLatin1() ==  coins.at(i).toString().toLatin1() ){
                    qDebug() << "index" << query3.value(0).toString() << "removing coin from rcoins " << query3.value(1).toString();
                    query3.exec("DELETE FROM coins WHERE origindex =" "'"+query3.value(0).toString()+"'");
                }
            }
            //  query3.clear();
        }
        qDebug()<<"Successful rcoins database connection";
        QSqlDatabase::database().commit();
        db.close();
        coins.clear();
        query3.clear();



    }
    //pull coins from wallet or yearlydb's and place into file to be processed
    if (ruserid.toLatin1() == ""){ // if receiver is blank then check admin mode
        if (admin){ // send to user already done with placecoins
            //     placecoins();

        }else{ // inbetween personal accounts from main wallet ID or pull from all addresses.  might not be needed use send button instead

            //check if wallet exists otherwise error message wallet non existant



        }
    } else { //send from address to address check if any of wallet addresses match for multiple wallets the same for timed offline transactions

        //   db.setDatabaseName("./"+ result +".sqlite");
        //          db.setDatabaseName("./wallet.sqlite");
        //       db.open();
        //           QSqlDatabase::database().transaction();
        //           QSqlQuery query;
        //           query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
        //          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
        //           while (query.next()) {
        //                  coins << query.value(0).toString();
        //           }
        //           QSqlDatabase::database().commit();
        //       db.close();

    }

    QFile file2("tmptx.txt");
    if (!file2.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // Handle the error if the file cannot be opened.
        qDebug() << "Failed to open the file for reading and writing:" << file2.errorString();
        //return;
    }

    QTextStream stream(&file2);

    // Writing data to the stream.
    stream << "sender:" << suserid.toLatin1();  // sender
    stream << "receiver:" << ruserid.toLatin1();
    //use users ekey and encrypted public signedcoins list? encrypted coins to verify sender to server also can be stored like that in others wallets
    stream << "ammount:"<< etxcoins; // ammount

    QDateTime currentDateTime = QDateTime::currentDateTime();
    stream << "datetime:" << currentDateTime.toString("dd.MM.yyyy'T'HH:mm:ss");
    file2.close();
    // Reopen the file in append mode to add the checksum.
    if (!file2.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Could not open file for appending:" << file2.errorString();
        //return;
    }
    // Calculate and write the MD5 checksum of the file.
    //file2.seek(0); // Go to the beginning of the file to read its contents for the checksum.
    QString md5Checksum = fileChecksum(file2.fileName(), QCryptographicHash::Md5);
    stream << "md5:" << md5Checksum;

    qDebug() << "Document saved successfully.";
    file2.close();

    return 0;



    //generate random transaction key maybe with time place with tx plain and then encrypt coins with it.
}

QString MainWindow::generateRXfile(QString ruserid,QString suserid,QString etxcoins){ //rxfile to give client encrypted coins to put in wallet might not be needed. for client only cold server


    // generating rx file will allow client and backend to receive coins\

    //etxcoins is either tmptxtfile with encryptedcoins or to make more secure use memory
    //encrypt coin addresses with mastercoinkey and userid to send the coins to their wallets if so desired (upto the user for extra security also gives ability to send between user accounts)
    //verify with time encrypted password verify and send time info password encrypted with userid
    // do they get their actual userid or an encrypted version based on masterkey and their password

    // use these to generate client wallets too aswell as processing on server / forward to server

    QString fileName2;
    QString coins;
    if (admin){ // maybe check if frontend or backend
        fileName2="./rxtx/tmp.txt";
    }else {
        fileName2 = QFileDialog::getSaveFileName(this,  tr("Save TX"), "",  tr("SaveRX/TX File (*.txt);;All Files (*)"));
        QFile file(fileName2);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            // Handle the error if the file cannot be opened.
            qDebug() << "Failed to open the file for reading and writing:" << file.errorString();
            //return;
        }

        QTextStream stream(&file);

        // Writing data to the stream.
        stream << "sender:" << suserid.toLatin1() << "\n";
        stream << "receiver:" << ruserid.toLatin1() << "\n";
        stream << "amount:" << etxcoins << "\n";
        QDateTime currentDateTime = QDateTime::currentDateTime();
        stream << "datetime:" << currentDateTime.toString("dd.MM.yyyy'T'HH:mm:ss") << "\n";


        file.close();


    }

    //QFile file("./newtx.txt"); // maybe do this in memory later or send with smtp
    //if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    //{
        //QTextStream stream(&file);
        //stream << "sender:" << suserid.toLatin1();  // sender
        //stream << "receiver:" << ruserid.toLatin1();
        //use users ekey and encrypted public signedcoins list? encrypted coins to verify sender to server also can be stored like that in others wallets
        //stream << "ammount:"<< etxcoins; // ammount

        //QTime starttime(QTime::currentTime().hour(),QTime::currentTime().minute());
        //QDate dNow(QDate::currentDate());

        //stream << "datetime:"<< dNow.toString("dd.MM.yyyy")+"T"+starttime.toString();
        //file.close();


    //} else {
    //    qDebug() << "Failed to open the file: " << file.errorString();
    //}

    //encrypt file check md5sum
    //stream << "md5:" << fileChecksum("tmptx.txt",QCryptographicHash::Md5); // md5sum
    //append md5sum

    // if frontend admin mode then use its own key
    return 0;

}



void MainWindow::Compress(QString saveFile , QString ofilename)
{
#ifdef QUAZIP
    //QString saveFile = QFileDialog::getSaveFileName(this, "Select file to save","", "Zip File(*.zip)");
    QStringList list;
    if(JlCompress::compressFiles(saveFile, list)){    }
#endif
}

void MainWindow::unCompress(QString zipFile , QString outdir)
{
#ifdef QUAZIP
    if(zipFile == "")
        return;

    // QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
    //               "",
    //            QFileDialog::ShowDirsOnly
    //            | QFileDialog::DontResolveSymlinks);
    if(outdir == "")
        return;

    QStringList list = JlCompress::getFileList(zipFile);
    JlCompress::extractFiles(zipFile, list, outdir);
#endif
}



QString MainWindow::validateCOINsignWallet(QString ID,QString Coin){ //for offline server validation

    //match user and coin
    //  if (offline){
    db.setDatabaseName("./db/pickup.sqlite");
    db.open();
    db.transaction();
    QSqlQuery query;
    query.exec("SELECT * FROM " "'" +ID.toLatin1()+"'");
    while (query.next()) {
        QString coinid = query.value(1).toString();
        if (coinid.toLatin1() == Coin.toLatin1())
            return Coin.toLatin1();
    }
    db.commit();
    db.close();
    //}
    return "notvalid";
}

QString MainWindow::validateCOINsign(QString coin,QString userID,QString password){
    // for getting coins from rcoins and placing into userid
    //used for signing coins with userid and password and index,datetime
    //encrypt coin during validation with user password then return coin address

    qDebug() << "looking for coin" << coin.toLatin1();
    QString ekey = password;
    // int euserid;
    // QString password;
    QString datetime;

    // check if validating signed or unsigned coin
    if (userID.toLatin1() == ""){ // only check rcoins and coins for unencrypted coins
        //check indexes match in coinsdb and rcoins

        //rencryptedcoins public coinlookup list (validationcoinkeys from coinkeypublic)


        db.setDatabaseName("wallet.sqlite");
        db.open();
        db.transaction();
        QSqlQuery query4;
        query4.exec("SELECT * FROM users WHERE name = " "'" + userID.toLatin1() + "'" " AND addr = "+coin.toLatin1());
        while (query4.next()) {
            //   yeardb = query.value(0).toInt();
            qDebug() << "coin " << query4.value(0).toString();
            // return yeardb.toLatin1();
        }
        db.commit();
        db.close();



        db.setDatabaseName("availableCoins.sqlite"); // random coins open
        db.open();
        QSqlDatabase::database().transaction();
        QSqlQuery query5;
        query5.exec("SELECT * FROM coins WHERE addr=""'"+coin.toLatin1()+"'");
        while (query5.next()) {
            qDebug() << "coin found" << query5.value(0).toString();
        }
        QSqlDatabase::database().commit();
        db.close();

        //          db.setDatabaseName("rencryptedcoins.sqlite");
        //          db.open();
        //          db.transaction();
        //              QSqlQuery query4;
        //              query4.exec("SELECT * FROM users WHERE name = " "'" + userID.toLatin1() + "'" " AND addr = "+coin.toLatin1());
        //              while (query4.next()) {
        //               //   yeardb = query.value(0).toInt();
        //                  qDebug() << "coin " << query4.value(0).toString();
        //                 // return yeardb.toLatin1();
        //              }
        //          db.commit();
        //          db.close();

    }else{        ///check user exists and get signing info to sign coins with
        qDebug() << "signing coin then getting another";
        if (coin.length() > ui->coinlength->text().toInt() ){ //8 if encrypted unsign coin settingsforcoinlength
            qDebug() << "encrypted coin during verify unsingning";
            db.setDatabaseName("wallet.sqlite"); //search for signed coin in db then unsign for placement and resigning
            db.open();
            QSqlDatabase::database().transaction();
            QSqlQuery query2;
            query2.exec("SELECT * FROM users WHERE coin = ""'"+userID.toLatin1()+"'");
            while (query2.next()) {
                //userid = query.value(1).; //not encrypted with user password
                ekey = query2.value(7).toString();
                password = query2.value(4).toString();
                datetime = query2.value(6).toString(); //datetime
                qDebug() << "user " << userID.toLatin1() << " pass " << password << "ekey " << ekey;
                //qDebug() << datetime;
                //  return yeardb;
            }
            QSqlDatabase::database().commit();
            db.close();

            //   signedcoin = simplecrypt(ecoinuser,ekey.toLatin1(),QCryptographicHash::Sha512);
            //   QString ecoinuser =  simplecrypt(euserID.toLatin1(),ecoin.toLatin1(),QCryptographicHash::Sha512);
            //   QString ecoin =  simplecrypt(signedcoin.toLatin1(),coinkey.toLatin1(),QCryptographicHash::Sha512);
        }


        if (admin){ // validate with all the extra databases
            QString euserID;
            db.setDatabaseName("database.sqlite");
            db.open();
            QSqlDatabase::database().transaction();
            QSqlQuery query2;
            query2.exec("SELECT * FROM users WHERE userid = ""'"+userID.toLatin1()+"'");
            while (query2.next()) {
                euserID = query2.value(2).toString(); //not encrypted with user password
                ekey = query2.value(7).toString();
                password = query2.value(4).toString();
                datetime = query2.value(6).toString(); //datetime
                qDebug() << "user " << userID.toLatin1() << " pass " << password << "ekey " << ekey;
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
                        QString ecoin =  simplecrypt(signedcoin.toLatin1(),coinkey.toLatin1(),QCryptographicHash::Sha512);

                        qDebug() << "signedcoin: " << signedcoin.toLatin1();
                        qDebug() << "coinkeysigned coin: " << ecoin.toLatin1();
                        QString ecoinuser =  simplecrypt(userID.toLatin1(),ecoin.toLatin1(),QCryptographicHash::Sha512);

                        qDebug() << "usersigned coin: " << ecoinuser.toLatin1();
                        signedcoin = simplecrypt(ecoinuser.toLatin1(),ekey.toLatin1(),QCryptographicHash::Sha512);
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
            //qDebug() << 'not proper userid';
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

    return userid.left(4);
}



#endif // VALIDATECOINS_H
