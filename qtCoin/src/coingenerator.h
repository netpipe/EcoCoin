#ifndef COINGENERATOR_H
#define COINGENERATOR_H
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


QString MainWindow::getClientAddress(){ //change to getclientaddress instead
    int length = 8;
    QString clientaddress = year + GetRandomString(length,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
    return clientaddress;
}

void MainWindow::selectUsersCoins(QString userid,QString yeardb) // not ready yet
{
    db.setDatabaseName("./db/"+yeardb+".sqlite");
    if(db.open())
    {
       qDebug() << "Successful database connection";
    }
    else
    {
       qDebug() << "Error: failed database connection";
    }


    QString query;
    query.append("SELECT * FROM "+userid);

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug() << "The user is properly selected";
    }
    else
    {
        qDebug() << "The user is not selected correctly";
        qDebug() << "ERROR! "<< select.lastError();
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



void MainWindow::generateRCoins()
{
    //read coins.txt and send them to new availablecoins database
        QFile MyFile("coins.txt");
        MyFile.open(QIODevice::ReadOnly);
        QTextStream in (&MyFile);
       // MyFile.seek(MyFile.size() ); /// qrand() % 10
        MyFile.seek(0 );
        QString line;
        QStringList list;
        QStringList nums;

        QVariantList coins;
        QVariantList index;
        qDebug() << "filling coins list";
        while (in.readLineInto(&line)) {
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
          //      nums.append(line);
                    index << list.at(0).toLatin1();
                    coins << list.at(1).toLatin1();
                   // qDebug() << list.at(1).toLatin1();
                   // coins << line.toLatin1();
            //        query += "INSERT INTO coins(addr) VALUES ('" + _coins[k] + "');";

         }

    createFreeCoinTable("rcoins.sqlite");
    qDebug() << "generating availablecoins";
    //sqlite create randomized availablecoins
    db.setDatabaseName("rcoins.sqlite");
    if(db.open())
    {
        qDebug() << "Successful coin database connection";
    }
    else
    {
        qDebug() << "Error: failed database connection";
    }

    db.transaction();

    QString query2 = "INSERT INTO coins(origid,addr) VALUES (?,?)";

//    qDebug() << query;
    QSqlQuery insert;
    insert.prepare(query2);
    insert.addBindValue(index);
    insert.addBindValue(coins);

    if(insert.execBatch())
    {
        qDebug() << "Coin is properly inserted";
    }
    else
    {
        qDebug()<<"ERROR! "<< insert.lastError();
    }
    db.commit();
    index.clear();
    coins.clear();
    insert.clear();
    db.close();


    //generate md5sum
    QByteArray coinstxtmd5 =  fileChecksum("rcoins.sqlite",QCryptographicHash::Md5);

    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
   // codec->toUnicode(coindb)

//    QFile hashfile("hashes.txt");
//    if(hashfile.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        QTextStream stream(&hashfile);
//            //hashfile.seek(0);
//            stream << "coinstxt:" << coinstxtmd5.toHex() << endl;
//            stream << "coinsdb:" << coindb.toHex() << endl;
//            stream << "availableCoins:" << availablecoins.toHex() << endl;
//       }
//    hashfile.close();

}


void MainWindow::createCoinTable(QString DBname)
{
   // db.setDatabaseName("coins.sqlite");
    db.setDatabaseName(DBname.toLatin1());
        //db.setDatabaseName("avalableCoins.sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    QString query;
    query.append("CREATE TABLE IF NOT EXISTS coins("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "addr VARCHAR(50));");

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

void MainWindow::createFreeCoinTable(QString DBname)
{
    db.setDatabaseName(DBname.toLatin1());

    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    QString query;
    query.append("CREATE TABLE IF NOT EXISTS coins("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "origid VARCHAR(50),""addr VARCHAR(50));");

//    query.prepare("INSERT INTO employee (id INTEGER PRIMARY KEY AUTOINCREMENT, origid , addr) "
//                  "VALUES (:id, :origid, :coin)");
//    query.prepare("INSERT INTO employee (id, origid, addr) "
//                  "VALUES (?, ?, ?)");

//    query.addBindValue(1001);
//    query.addBindValue("Thad Beaumont");
//    query.addBindValue(65000);

//    query.bindValue(":id", 1001);
//    query.bindValue(":origid", "Thad Beaumont");
//    query.bindValue(":addr", 65000);

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



void MainWindow::RandomizeCoins()
{
//put into availableCoins.db

    //read coins.db into memory to try and open both sqldb at same time or write to textfile then read back in
    db.setDatabaseName("coins.sqlite");
    QStringList tables = db.tables();
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

 qDebug() << "randomizing";

    QSqlQuery query;

    query.prepare("SELECT * FROM coins ORDER BY random()");

    QFile file("coins.txt");
      //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          if(file.open(QIODevice::ReadWrite | QIODevice::Text))
          {
            QTextStream stream(&file);
              if(!query.exec())
             {
                  qCritical() << query.lastQuery();
                  qCritical() << query.lastError().databaseText();
                  qCritical() << query.lastError().driverText();
                  return;
              }

              query.next();
              while(query.isValid())
              {
                  stream << query.value("id").toString() << ":" << query.value("addr").toString() <<"\n";
                  query.next();
                //  qDebug() << "inserting coin" << query.value("id").toString() << ":" << query.value("addr").toString();
              }
   }
          file.close();
    //db.commit();
    query.clear();
    db.close();

    createCoinTable("availableCoins.sqlite");
    //read coins.txt and send them to new availablecoins database
        QFile MyFile("coins.txt");
        MyFile.open(QIODevice::ReadOnly);
        QTextStream in (&MyFile);
       // MyFile.seek(MyFile.size() ); /// qrand() % 10
        MyFile.seek(0 );
        QString line;
        QStringList list;
        QStringList nums;

        QVariantList coins;
        QVariantList index;
        qDebug()<<"filling coins";
        while (in.readLineInto(&line)) {
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
          //      nums.append(line);
                  //  index << list.at(0).toLatin1();
                    coins << list.at(1).toLatin1();
                   // qDebug() << list.at(1).toLatin1();
                   // coins << line.toLatin1();
            //        query += "INSERT INTO coins(addr) VALUES ('" + _coins[k] + "');";

      }

   //  coins << QVariant(QVariant::String);
    //index << QVariant(QVariant::String);

    qDebug()<< "generating availablecoins";
    //sqlite create randomized availablecoins
    db.setDatabaseName("availableCoins.sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    db.transaction();

    QString query2 = "INSERT INTO coins(addr) VALUES (?)";

//    qDebug() << query;
    QSqlQuery insert;
    insert.prepare(query2);
    insert.addBindValue(coins);

    if(insert.execBatch())
    {
        qDebug() << "Coin is properly inserted";
    }
    else
    {
        qDebug()<<"ERROR! "<< insert.lastError();
    }
    db.commit();
    coins.clear();
    insert.clear();
    db.close();

    writeAdminFrontendHashes();

}

void MainWindow::insertCoins()
{
    //https://stackoverflow.com/questions/1711631/improve-insert-per-second-performance-of-sqlite
    //https://forum.qt.io/topic/86846/insert-data-into-sqlite-is-very-slow-by-qsqlquery/5
    //https://stackoverflow.com/questions/31197144/why-is-my-sqlite-query-so-slow-in-qt5/31208237
    db.setDatabaseName("coins.sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    db.transaction();
    QString query = "INSERT INTO coins(addr) VALUES (?)";
    QVariantList coins;
    for(int k = 0 ; k < _coins.count() ; k++)
    {
        coins << _coins[k];

//        query += "INSERT INTO coins(addr) VALUES ('" + _coins[k] + "');";
    }
   // coins << QVariant(QVariant::String);

//    qDebug() << query;
    QSqlQuery insert;
    insert.prepare(query);
    insert.addBindValue(coins);

    if(insert.execBatch())
    {
        qDebug() << "Coin is properly inserted";
    }
    else
    {
        qDebug()<<"ERROR! "<< insert.lastError();
    }
    db.commit();
    _coins.clear();
        insert.clear();
    db.close();
}

int MainWindow::md5verifydb(){
// md5sum coinsdb maybe choose semirandom coinsammount to make md5 more unique
// save with settings.txt
//verify databases

    //encrypt hashes for extra security store them on thumbdrive too also keep plaintext unencrypted versions on local

    QFile MyFile("hashes.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
    QStringList list;
    QStringList nums;

    do {
        line = in.readLine();
        QString searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());

       QString coinstxtmd5=nums.at(0);
       ui->coinname->setText(coinstxtmd5.toLatin1());
       qDebug("%s", qUtf8Printable(coinstxtmd5));

       QString coindb=nums.at(1);
       ui->coinname->setText(coindb.toLatin1());
       qDebug("%s", qUtf8Printable(coindb));

       QString availablecoins=nums.at(2);
       ui->coinname->setText(availablecoins.toLatin1());
       qDebug("%s", qUtf8Printable(availablecoins));

       QByteArray coinstxtmd52 =  fileChecksum("coins.txt",QCryptographicHash::Md5);
       QByteArray coindb2 =  fileChecksum("coins.sqlite",QCryptographicHash::Md5);
       QByteArray availablecoins2 =  fileChecksum("availableCoins.sqlite",QCryptographicHash::Md5);

       //QTextCodec *codec = QTextCodec::codecForName("KOI8-R");

       if (coinstxtmd5.toLatin1()==coinstxtmd52.toHex() && coindb.toLatin1()==coindb2.toHex() && availablecoins.toLatin1() == availablecoins2.toHex()  )
       {
//           QMessageBox Msgbox;
//               Msgbox.setText("verified ");
//               Msgbox.exec();
            return 1;
       }

return 0;
    //md5 convert coinsdb to randomcoins.db then md5sum can also check freecoins.db after each tx
}


void MainWindow::on_pushButton_clicked() //generate coins button
{

    if((ui->coincount->text().toInt() & 1) == 0){ // check if odd or even

        QFile Fout("coins.sqlite");
        if(Fout.exists())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Already Exists", "do you want to generate new coindb",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
              qDebug() << "Yes was clicked";
              cleartablesusers();
           //   QApplication::quit();
            } else {
              qDebug() << "no";
              return;
            }
        }
        Fout.close();

    createCoinTable("coins.sqlite");
    coini=0;
    gentotext=0; // use 0 for sql
    GenerateCoins3(ui->coinlength->text().toInt(),ui->coincount->text().toInt());
    RandomizeCoins();
    generateRCoins();
    QString tester1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    masterkey = GetRandomString(12,tester1.toLatin1());
    QThread::usleep(8000);
    coinkey = GetRandomString(12,tester1.toLatin1());

    QMessageBox Msgbox;
       // int sum;
       // sum = ui->coincount->text().toInt();
        Msgbox.setText("coin created: ");
        Msgbox.exec();
   }else{
    QMessageBox Msgbox;
       // int sum;
       // sum = ui->coincount->text().toInt();
        Msgbox.setText("needs to be even number: ");
        Msgbox.exec();
    }
    on_gencoininfo_btn_clicked(); // save info
}

void MainWindow::generateCoins() //puts coins in text file to be read in by randomizer
{
  //  qDebug() << "generating coins to textfile";

    QFile file("coins2.txt");
    if(file.open(QIODevice::ReadWrite |  QIODevice::Append | QIODevice::Text))// QIODevice::Append |
    {
        QTextStream stream(&file);
       // file.seek(file.size());
        file.seek(0);
        for(int i=0 ; i < _coins.count() ; i++)
        {
            stream << QString::number(coini) << ":" <<_coins[i] << endl;
            //stream <<_coins[i] << endl;
            coini++;
        }
    }
        _coins.clear();
        file.close();

}

void MainWindow::GenerateCoins3(int length,int total)
{
    QString arrm = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    QString arr= GetRandomString(15,arrm.toLatin1());
    //qDebug()<< "coinstring" << arr;
    coinkey=arr;
    //save string incase more coins need to be added after
    QString data;
   // _total = 10000;
    _total = total;
    _count = 0;
    _state = false;
 //   _length = 8;
    _length = length;

    for (int i=0; i < _length; i++){
       data += " ";
    }

     qDebug() << "running combo util";

    combinationUtil(arr, arr.length(), _length, 0, data, 0);
}

void MainWindow::combinationUtil(QString arr, int n, int r, int index, QString data, int i)
{
    if(_state)
        return;
    if(index == r)
    {
        //qDebug() << data;

        // write to the database
        _coins.append(data);
        if (gentotext == 1){
            if(_coins.count() >= _total/10) // misses if there arnt more than 80 coins so need to generatelastbit
            {
             //    qDebug() << _total/2;
                generateCoins(); //textversion
            }
        }else{
            if(_coins.count() >= _total/10)
            {
            insertCoins(); //sqlversion
            // qDebug() << _total/2;
            }
        }
        _count++;
        if(_count >= _total)
        {
            _state = true;
        }
        return;
    }

    if(i >= n)
        return;

    data[index] = arr[i];
//    data.at(index) = arr.at(i);

    combinationUtil(arr, n, r, index+1, data, i+1);
    combinationUtil(arr, n, r, index, data, i+1);
}

//void MainWindow::GenerateCoins2() //not used
//{ //basic demo-bruteforce algorithm in C++ from hacksenkessel.com
//    QString alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"; // can randomize string and record as part of the unique coin signature ?
//    const int min_pw_len = 1;
//    const int max_pw_len = 3;
//    char pw[max_pw_len + 2];
//    int n, pos = min_pw_len -1, count=0;
//    long double max_perms = 0;
//    string final;
//    /* create last possible password based on max. password length
//     * and alphabet,  i.e. 'zzzz' for an alphabet [a-z] with 4 chars
//     */
//    for (int k=0; k<max_pw_len; k++) final+=alphabet[alphabet.length()-1];
//    /* calculate number of permutations to run by interpreting the
//     * final password as polynomial. Be careful, this is where an integer
//     * overflow can occur by exceeding the max. password length and/or the alphabet
//     */
//    for (int n=0; n < (int) final.length(); n++)
//     max_perms += (long double) ( (long double) (alphabet.find_first_of(string(1, final[n]))+1) * powl(alphabet.length(), final.length()-n-1));
//    /* Print out calculated information and set password
//     * to first char in alphabet
//     */
//    cout << "Last password: " << final << endl << "Permutations:  " << max_perms << endl << endl;
//    cout << "Starting bruteforce:" << endl;
//    memset(pw, '\0', sizeof(pw));
//    pw[0] = alphabet[0];
//    /* bruteforce until previously calculated max. number
//    * of permutations is exceeded */
//    while (count < max_perms) {
//    /* iterate last password index separately, as we know its number
//     * of iterations matches the alphabet's length
//     */
//         for (int a=0;  a < (int) alphabet.length(); a++) {
//            pw[pos] = alphabet[a];
//            cout << pw << endl;
//            if (count++ >= max_perms) break;
//         }
//  /* iterate remaining indexes in descending order, as in
//   * all indexes except for the last index we already iterated
//   */
//  for (n = pos; n>=0; n--) {
//    if (pw[n] != alphabet[alphabet.length() -1]) {
//      pw[n]++;
//      break;
//    }
//    else {
//      if (n == 0) {
//        /* increase password length */
//        memset(pw, (int) alphabet[0], ++pos + 1);
//        break;
//      }
//      /* re-initialize current password index */
//      pw[n] = alphabet[0];
//    }
//  }
//}
//}

//void MainWindow::GenerateCoins1() //not used
//{   //bruteforce algo by Author: Neo_Fr


//    //bruteforce generate coin numbers
//    //randomize indexs after backing up into coinlist.db into another db called freecoins.db
//    //stop after set ammount of coins have been generated

//    int LongMin;
//    int LongMax;
//    unsigned long nbMDP = 0;

//    //const char ALL[105] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz1234567890<>,?;.:/!§*µù%$£¤¨+=})]à@ç^\\_`è|-[({'#\"é~&";
//    const char ALL[105] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"; // can randomize string and record as part of the unique coin signature ?
//    int Lg = LongMin - 1;
//    int i, x, y;

//    char* Buff = (char*) malloc(Lg); // Creer le Buff de Lg octet
//    while(Lg != LongMax)
//    {
//        realloc(Buff, Lg); // Augmente la taille du buff
//        int Nchar[Lg];
//        for(i=0; i<=Lg; i++)
//        {
//            Buff[i] = ALL[0]; // Remplie le Buff du premier char
//            Nchar[i] = 0; // Initialise le tableau de int a 0
//        }
//        while(Nchar[0] != 105)
//        {
//             for(x=0; x<=105; x++)
//             {
//                     Buff[Lg] = ALL[x];
//                     Nchar[Lg] = x;
//                     printf("%s \n", Buff);
//                     nbMDP++;
//             }
//             for(y=Lg; y>=0; y--)
//             {
//                 if((Nchar[y] == 105)&&(Nchar[0]!= 105))
//                 {
//                     Nchar[y] = 0;
//                     Nchar[y-1]++;
//                 }
//                 Buff[y] = ALL[Nchar[y]];
//             }
//       }
//       Lg++;
//    }
//    free(Buff);// Libere la memoire
//}

#endif // COINGENERATOR_H
