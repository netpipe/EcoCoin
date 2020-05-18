#ifndef COINFUNCTIONS_H
#define COINFUNCTIONS_H
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
        QString result = validateID(ui->givecoinsid->text().toLatin1()).toLatin1(); // result will be unencrypted id
        qDebug() << "validate" << result;

        float balance = checkBalance(result.toLatin1());
        qDebug() << "balance:" << balance;


        //    //find random coin and insert it ammount times
        //generateTXfile(result, etxcoins);  // use generated tx
        placeCoins(result.toLatin1(),ui->givecoinsammount->text()); // send encrypted id

            float balance2 = checkBalance(ui->givecoinsid->text().toLatin1());
            float total = ui->givecoinsid->text().toFloat() + balance;


    QMessageBox Msgbox;
        Msgbox.setText("coins sent ");
        Msgbox.exec();
} else {
//send user to user coins
        //if client mode then generatetxfile
        int admin=0;

        QString result = validateID(ui->givecoinsid->text().toLatin1()).toLatin1(); // result will be unencrypted id
        qDebug() << "validate" << result;

        QString result2 = validateID(ui->givecoinsid2->text().toLatin1()).toLatin1(); // result will be unencrypted id
        qDebug() << "validate" << result2;

    if(admin){
        float balance = checkBalance(result.toLatin1());
        qDebug() << "balance:" << balance;

        placeCoins(result,"receive"); //send to rcoins

        placeCoins(result2,ui->givecoinsammount->text()); //place into user account after validating id

    }else{

        generateTXfile(result,result2,ui->givecoinsammount->text());

    }

    }
 }
 //    //find user in yearly db pull coins out and verify validity then place back into rcoins
 //    //re-md5sum file

}

int MainWindow::placeCoins(QString euserid,QString ammount) //free coins from coins.db
{// place into client walled based on yearly tables when created ID

///////////////////
//    QString test=md5Checksum("testing123");//
//    qDebug() << "md5sum:" << test.toLatin1();

//    QByteArray array;
//    array.append("ABCDEF12343");
//    qDebug() << QString(array.toHex());
//////////////////////
//ui->givecoinsammount.text().toLatin1()
    if (euserid.toLatin1() == "receive"){ //receiving coins
//get ammount of usercoins, verify then decrypt and add to rcoins and remove from user table
        QVariantList coins;
        QVariantList origindex;
        //ui->receiveid->text().toLatin1()
        if (ui->receiveid->text().size() > 12){
            //decrypt

        }
        db.setDatabaseName("./db/"+ yeardb.toLatin1() +".sqlite");
        db.transaction();
        db.open();
            QSqlQuery query;
            query.exec("SELECT * FROM ""'"+ ui->receiveid->text().toLatin1()+"'"" LIMIT "+ammount.toLatin1());
            while (query.next()) {
                origindex << query.value(1).toString();
                coins << query.value(3).toString();
                qDebug() << "picked coins" << query.value(2).toString();
            }
            db.commit();
        db.close();

        //check all coins valid then place back into rcoins
        // if ( validateCOINsign(coins.at(i).toString()) == "valid"){
        //        qDebug() << "coin is already signed"
        //                    return 1;
        //}else {
        //                    qDebug << signedcoin; };


//        for (int i=0; i < ammount.toInt();i++){
//            //unsigncoin();
//        }


        qDebug()<< "inserting coins into rcoins";
        //sqlite create randomized availablecoins
        qDebug() << yeardb;

        QVariantList signedcoins;
        int missingcoin=0;
        qDebug() << "unsign and validate coins";
        //int i2=0;
        //for (int i=0; i < coins.size(); i++){

            for (int i=0; i < ammount.toInt(); i++){
            QString test = validateCOINsign( coins.at(i).toString(), "" ).toLatin1(); //validate without id and get from textbox
            if (test != ""){ // coin not from rcoins needs decryption first
            signedcoins << test ;
            } else { missingcoin = 1;}
            }


        db.setDatabaseName("rcoins.sqlite");
        if(db.open())        {            qDebug()<<"Successful coin database connection";        }
        else        {            qDebug()<<"Error: failed database connection";        }

        db.transaction();

        QString query2 = "INSERT INTO coins(origindex,addr,datetime,class) VALUES (?,?,1,0)";

    //    qDebug() << query;
        QSqlQuery insert;
        insert.prepare(query2);
        insert.addBindValue(origindex);
        insert.addBindValue(signedcoins);

        if(insert.execBatch())
        {            qDebug() << "Coin is properly inserted";        }
        else        {            qDebug()<<"ERROR! "<< insert.lastError();      }
        db.commit();
       // index.clear();
     //   coins.clear();
        insert.clear();
        signedcoins.clear();
        db.close();


    } else{ ///send coins to yeardb wallet

    QVariantList coins;
    QVariantList origindex;
    db.setDatabaseName("rcoins.sqlite");
    QSqlDatabase::database().transaction();
    db.open();
        QSqlQuery query;
        query.exec("SELECT * FROM coins ORDER BY random() LIMIT "+ammount.toLatin1());
        while (query.next()) {
        coins << query.value(2).toString();
        origindex << query.value(0).toString();
        qDebug() << "picked coins" << query.value(2).toString();
        }
        QSqlDatabase::database().commit();
    db.close();


//verify coins and insert into yearly userid
QVariantList signedcoins;
int missingcoin=0;
qDebug() << "validate coins";
//int i2=0;
//for (int i=0; i < coins.size(); i++){

    for (int i=0; i < ammount.toInt(); i++){
    QString test = validateCOINsign( coins.at(i).toString(), euserid.toLatin1() ).toLatin1();
    if (test != ""){ // coin not from rcoins needs decryption first
    signedcoins << test ;
    } else { missingcoin = 1;}



}

    //valid user so it must have table already in yeardb
    qDebug()<< "inserting coins into yeardb";
    //sqlite create randomized availablecoins
    qDebug() << yeardb;

    db.setDatabaseName("./db/"+ yeardb.toLatin1() +".sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }
    db.transaction();

    QString query2 = "INSERT INTO ""'"+euserid.toLatin1()+"'""(origindex,addr,datetime,class) VALUES (?,?,1,0)";

//    qDebug() << query;
    QSqlQuery insert;
    insert.prepare(query2);
    insert.addBindValue(origindex);
    insert.addBindValue(signedcoins);

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
 //   coins.clear();
    insert.clear();
    signedcoins.clear();
    origindex.clear();
    db.close();

    if ( missingcoin == 0 ) { // if no missing coins validated then remove from rcoins
    qDebug() << "no missing coins validated removing coins from rcoins.db";

     db.setDatabaseName("rcoins.sqlite");
       db.open();
       QSqlDatabase::database().transaction();
           QSqlQuery query3;
        for (int i=0; i < ammount.toInt(); i++ ) {
            query3.exec("SELECT * FROM coins WHERE addr LIKE " "'" + coins.at(i).toString().toLatin1() + "'" "ORDER BY random()");
            while (query3.next()) {
               //  qDebug() << "rcoin " << query3.value(2).toString();
               //  qDebug() << "rcoin2 " << coins.at(i).toString().toLatin1();
               if ( query3.value(2).toString().toLatin1() ==  coins.at(i).toString().toLatin1() ){
                    qDebug() << "index" << query3.value(0).toString() << "removing coin from rcoins " << query3.value(2).toString();
                    query3.exec("DELETE FROM coins WHERE id =" "'"+query3.value(0).toString()+"'");
               }
            }
         //  query3.clear();
        }
        QSqlDatabase::database().commit();
        db.close();
        coins.clear();
        query3.clear();
    } else{
    qDebug() << "missing coins not removing from db";
    return 0;
    }

}
               //   "DELETE FROM euserid WHERE addr = "+ "OR StudentId = 12;"
                  // "UPDATE coins SET lastupdated WHERE userid=11"
                  // "DROP table" +userid
                  // "ALTER TABLE name ADD COLUMN test TEXT" or char(50)
                     // "ALTER TABLE name DROP COLUMN name"

}

float MainWindow::checkBalance(QString euserID){
//iterate and count coins from userid from yeardb
    float balance=0;

    db.setDatabaseName("./db/"+yeardb.toLatin1()+".sqlite");
    db.open();
    QSqlDatabase::database().transaction();
        QSqlQuery query;
      //  query.exec("SELECT * FROM coins WHERE name = ""'"+euserID+"'");
        query.exec("SELECT * FROM ""'"+euserID.toLatin1()+"'");
        if (query.next()) {
            balance++;
        }
    QSqlDatabase::database().commit();
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
        db.transaction();
            QSqlQuery query;
            query.exec("SELECT * FROM coins");
            while (query.next()) {
             //   int employeeId = query.value(0).toInt();
                qDebug() << query.value(2).toString(); //addr
                qDebug() << query.value(1).toInt(); //origindex
                qDebug() << query.value(0).toInt(); //index
                coins++;
            }
            db.commit();
        db.close();

  return coins;
}



int MainWindow::checkAvailableCoins(QString db2,QString needed){
    //check available coins has enough for tx
        int coins=0;
        qDebug() << "checking enough coins available for tx";

        db.setDatabaseName(db2.toLatin1());
        db.open();
        db.transaction();
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
            db.commit();
        db.close();

  return coins;
}


//void MainWindow::DeleteColFromTable(QString DbName, QString TableName, QString ColName){ //https://stackoverflow.com/questions/8442147/how-to-delete-or-add-column-in-sqlite
//    SQLiteDatabase db = openOrCreateDatabase(""+DbName+"", Context.MODE_PRIVATE, null);
//    db.execSQL("CREATE TABLE IF NOT EXISTS "+TableName+"(1x00dff);");
//    Cursor c = db.rawQuery("PRAGMA table_info("+TableName+")", null);
//    if (c.getCount() == 0) {

//    } else {
//        String columns1 = "";
//        String columns2 = "";
//        while (c.moveToNext()) {
//            if (ctransaction.getString(1).equals(ColName)) {
//            } else {
//                columns1 = columns1 + ", " + c.getString(1) + " " + c.getString(2);
//                columns2 = columns2 + ", " + c.getString(1);
//            }
//            if (c.isLast()) {
//                db.execSQL("CREATE TABLE IF NOT EXISTS DataBackup (" + columns1 + ");");
//                db.execSQL("INSERT INTO DataBackup SELECT " + columns2 + " FROM "+TableName+";");
//                db.execSQL("DROP TABLE "+TableName+"");
//                db.execSQL("ALTER TABLE DataBackup RENAME TO "+TableName+";");
//            }
//        }
//    }
//}

#endif // COINFUNCTIONS_H
