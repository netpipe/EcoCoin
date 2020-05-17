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
//                        QSqlQuery query2;
//                        query2.exec("SELECT * FROM employee WHERE name = ''");
//                        while (query2.next()) {
//        //                    int employeeId = query.value(0).toInt();

//        //                    query.exec("INSERT INTO project (id, name, ownerid) "
//        //                               "VALUES (201, '', "
//        //                               + QString::number(employeeId) + ')');
//                        }
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

for (int i=0; i > coins.length(); i++){
//if (coin > 8) // coin not from rcoins needs decryption first
    signedcoins << validateCOINsign( coins.at(i).toString(), euserid.toLatin1() );

// if ( validateCOINsign(coins.at(i).toString()) == "valid"){
//        qDebug() << "coin is already signed"
//                    return 1;
//}else {
//                    qDebug << signedcoin; };

}

    //valid user so it must have table already in yeardb
    qDebug()<< "inserting coins into yeardb";
    //sqlite create randomized availablecoins
    qDebug() << yeardb;
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

    QString query2 = "INSERT INTO ""'"+euserid+"'""(origid,addr,class) VALUES (?,?,0)";

//    qDebug() << query;
    QSqlQuery insert;
    insert.prepare(query2);
  //  insert.addBindValue(index);
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
    coins.clear();
    insert.clear();
    db.close();



    //not sure what this is for yet
 //    db.setDatabaseName("./"+ result +".sqlite");
//       db.open();
//           QSqlDatabase::database().transaction();
//           QSqlQuery query3;
//         //  query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
//          // query.exec("SELECT * FROM coins WHERE name = ""'"+ +"'");
//           while (query3.next()) {
//               int employeeId = query3.value(0).toInt();
//             //  rcoins <<      //decrypt coins and reencrypt for new user
//               //can place into text file to be sure then delete here// verify enough is available

//              // query.exec("DELETE FROM userid WHERE addr ="); //do this after the tx has been validated if sending from user to user
//                         //if sending from admin pull from rcoins

//               //   "DELETE FROM euserid WHERE addr = "+ "OR StudentId = 12;"
//                  // "UPDATE coins SET lastupdated WHERE userid=11"
//                  // "DROP table" +userid
//                  // "ALTER TABLE name ADD COLUMN test TEXT" or char(50)
//                     // "ALTER TABLE name DROP COLUMN name"
//           }
//           QSqlDatabase::database().commit();
//       db.close();

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

    if (euserID.toLatin1() == ""){ // only check rcoins and coins

    }else{
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

    //  QString yeardb;
    // coin
  //  qDebug() <<    datetime.mid(0,4);
  //  qDebug() <<     datetime.left(4);

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
}

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

//void MainWindow::DeleteColFromTable(QString DbName, QString TableName, QString ColName){ //https://stackoverflow.com/questions/8442147/how-to-delete-or-add-column-in-sqlite
//    SQLiteDatabase db = openOrCreateDatabase(""+DbName+"", Context.MODE_PRIVATE, null);
//    db.execSQL("CREATE TABLE IF NOT EXISTS "+TableName+"(1x00dff);");
//    Cursor c = db.rawQuery("PRAGMA table_info("+TableName+")", null);
//    if (c.getCount() == 0) {

//    } else {
//        String columns1 = "";
//        String columns2 = "";
//        while (c.moveToNext()) {
//            if (c.getString(1).equals(ColName)) {
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
