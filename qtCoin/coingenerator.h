#ifndef COINGENERATOR_H
#define COINGENERATOR_H
#include <mainwindow.h>
#include  <math.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
//#include <QMainWindow>

void MainWindow::RandomizeCoins()
{
//put into availableCoins.db

    QFile MyFile("coins.txt");
    MyFile.open(QIODevice::ReadOnly);
    QTextStream in (&MyFile);
    QString line;
    QStringList list;
    QStringList nums;
QVariantList coins;
QVariantList index;
    do {
        line = in.readLine();
        QRegExp rx("[:]");// match a comma or a space
        list = line.split(rx);
      //      nums.append(line);
                index << list.at(0).toLatin1();
                coins << list.at(1).toLatin1();
        //        query += "INSERT INTO coins(addr) VALUES ('" + _coins[k] + "');";

    } while (!line.isNull());

  coins << QVariant(QVariant::String);
index << QVariant(QVariant::String);
    //read coins.db into memory to try and open both sqldb at same time or write to textfile then read back in
    db.setDatabaseName("avalableCoins.sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    QString query;
    db.transaction();
    query.append("INSERT INTO coins(addr) VALUES (?)");

    //SELECT column FROM table
    //ORDER BY RAND()
    //LIMIT 1

    QSqlQuery create;

    create.prepare(query);
  //  create.addBindValue(index);
    create.addBindValue(coins);
    if (create.execBatch())
    {
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }
db.commit();
    query.clear();
    db.close();

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
    coins << QVariant(QVariant::String);

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

void MainWindow::generateCoins() //puts coins in text file to be read in by randomizer
{
  //  qDebug() << "generating coins to textfile";
    QFile file("coins.txt");
    if(file.open(QIODevice::ReadWrite |  QIODevice::Append | QIODevice::Text))// QIODevice::Append |
    {
        QTextStream stream(&file);
        file.seek(file.size());

        for(int i ; i < _coins.count() ; i++)
        {
            stream << QString::number(coini) << ":" <<_coins[i] << endl;
            coini++;
        }
    }
        _coins.clear();
        file.close();

      //  RandomizeCoins();

}

void MainWindow::GenerateCoins3(int length,int total)
{
    QString arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
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
   // QString data = "        ";


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

        if(_coins.count() > 80) //if using sql use 300
        {
            //insertCoins(); //sqlversion
            generateCoins(); //textversion
        }else{
            generateCoins();
           qDebug() << "putting in last little bit";
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

QString MainWindow::GetRandomString(int length)
{ //https://stackoverflow.com/questions/18862963/qt-c-random-string-generation/18866593
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = length; //12 // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

QString MainWindow::GenerateClientAddress(int length){
    //QDate year = new QDate;
   // QDate date = QDate::currentDate();
    QString clientaddress = year + GetRandomString(length);
    //double check against existing addresses after encrypting unless not specified
    //   QString clientaddress = QDate::currentDate().year() + GetRandomString(length);
    //QDate::currentDate().year();
   // qDebug() << clientaddress << "/n";
    return clientaddress;
}

void MainWindow::GenerateCoins2()
{ //basic demo-bruteforce algorithm in C++ from hacksenkessel.com

    //bruteforce generate coin numbers
    //randomize indexs after backing up into coinlist.db into another db called freecoins.db
    //stop after set ammount of coins have been generated

    const string alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"; // can randomize string and record as part of the unique coin signature ?


    const int min_pw_len = 1;
    const int max_pw_len = 3;
    char pw[max_pw_len + 2];
    int n, pos = min_pw_len -1, count=0;
    long double max_perms = 0;
    string final;
    /* create last possible password based on max. password length
 * and alphabet,  i.e. 'zzzz' for an alphabet [a-z] with 4 chars
 */
    for (int k=0; k<max_pw_len; k++) final+=alphabet[alphabet.length()-1];
/* calculate number of permutations to run by interpreting the
 * final password as polynomial. Be careful, this is where an integer
 * overflow can occur by exceeding the max. password length and/or the alphabet
 */
    for (int n=0; n < (int) final.length(); n++)
     max_perms += (long double) ( (long double) (alphabet.find_first_of(string(1, final[n]))+1) * powl(alphabet.length(), final.length()-n-1));
/* Print out calculated information and set password
 * to first char in alphabet
 */
    cout << "Last password: " << final << endl << "Permutations:  " << max_perms << endl << endl;
    cout << "Starting bruteforce:" << endl;
    memset(pw, '\0', sizeof(pw));
    pw[0] = alphabet[0];
/* bruteforce until previously calculated max. number
 * of permutations is exceeded */
    while (count < max_perms) {
  /* iterate last password index separately, as we know its number
   * of iterations matches the alphabet's length
   */
         for (int a=0;  a < (int) alphabet.length(); a++) {
            pw[pos] = alphabet[a];
            cout << pw << endl;
            if (count++ >= max_perms) break;
         }
  /* iterate remaining indexes in descending order, as in
   * all indexes except for the last index we already iterated
   */
  for (n = pos; n>=0; n--) {
    if (pw[n] != alphabet[alphabet.length() -1]) {
      pw[n]++;
      break;
    }
    else {
      if (n == 0) {
        /* increase password length */
        memset(pw, (int) alphabet[0], ++pos + 1);
        break;
      }
      /* re-initialize current password index */
      pw[n] = alphabet[0];
    }
  }
}
}

void MainWindow::GenerateCoins1()
{   //bruteforce algo by Author: Neo_Fr


    //bruteforce generate coin numbers
    //randomize indexs after backing up into coinlist.db into another db called freecoins.db
    //stop after set ammount of coins have been generated

    int LongMin;
    int LongMax;
    unsigned long nbMDP = 0;

    //const char ALL[105] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz1234567890<>,?;.:/!§*µù%$£¤¨+=})]à@ç^\\_`è|-[({'#\"é~&";
    const char ALL[105] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"; // can randomize string and record as part of the unique coin signature ?
    int Lg = LongMin - 1;
    int i, x, y;

    char* Buff = (char*) malloc(Lg); // Creer le Buff de Lg octet
    while(Lg != LongMax)
    {
        realloc(Buff, Lg); // Augmente la taille du buff
        int Nchar[Lg];
        for(i=0; i<=Lg; i++)
        {
            Buff[i] = ALL[0]; // Remplie le Buff du premier char
            Nchar[i] = 0; // Initialise le tableau de int a 0
        }
        while(Nchar[0] != 105)
        {
             for(x=0; x<=105; x++)
             {
                     Buff[Lg] = ALL[x];
                     Nchar[Lg] = x;
                     printf("%s \n", Buff);
                     nbMDP++;
             }
             for(y=Lg; y>=0; y--)
             {
                 if((Nchar[y] == 105)&&(Nchar[0]!= 105))
                 {
                     Nchar[y] = 0;
                     Nchar[y-1]++;
                 }
                 Buff[y] = ALL[Nchar[y]];
             }
       }
       Lg++;
    }
    free(Buff);// Libere la memoire
}



#endif // COINGENERATOR_H
