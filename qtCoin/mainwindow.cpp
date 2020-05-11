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

//https://doc.qt.io/qt-5/sql-sqlstatements.html
//https://www.techonthenet.com/mysql/select.php

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

//    media->setMedia("sound.mp3");
    player->setMedia(QMediaContent(), buffer);
    player->play();




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

  //  ui->createtime->setTime(starttime);

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
                    "name VARCHAR(100),"
                    "surname VARCHAR(100),"
                    "age INTEGER NOT NULL,"
                     "etype INTEGER NOT NULL,"
                     "ekey VARCHAR(100),"
                     "extra VARCHAR(100),"
                    "class INTEGER NOT NULL"
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
void MainWindow::createyearly(QString ownerID)
{
    //holds users generated from each new year and their coins pulled from rcoins.sqlite

    db.setDatabaseName("./db/"+year+".sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;

    query.append("CREATE TABLE IF NOT EXISTS "+ownerID.toLatin1()+"("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "addr VARCHAR(100),"
                    "datetime INTEGER NOT NULL,"
                    "class INTEGER NOT NULL"
                    ");");

//    query.append("CREATE TABLE IF NOT EXISTS test("
//                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                    "owner VARCHAR(100),"
//                    "addr VARCHAR(100),"
//                    "datetime INTEGER NOT NULL,"
//                    "class INTEGER NOT NULL,"
//                    "etype INTEGER NOT NULL,"
//                    "ekey VARCHAR(100),"
//                    "extra VARCHAR(100)"
//                    ");");


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

    if (ui->encrypted_yes->text() == "Yes" ){

    QByteArray bFname = EncryptMsg(ui->lineEditName->text(),"123456789", "your-IV-vector");
    QString mykey1 = BigInt2Str(m_e); //rsa keys
    QString mykey2 = BigInt2Str(m_n); //rsa keys

    query.append("INSERT INTO users("
                 "id,"
                    "name,"
                    "surname,"
                    "phone,"
                 "etype,"
                 "ekey,"
                 "extra,"
                    "class)"
                    "VALUES("
                    "'"+bFname+"',"
                    "'"+ui->lineEditSurname->text()+"',"
                    ""+ui->lineEditAge->text()+","
                    ""+ui->lineEditClass->text()+""
                    ");");

//    "etype INTEGER NOT NULL,"
//    "ekey VARCHAR(100),"
//    "extra VARCHAR(100)"

     qDebug()<<bFname+ "/n";
    }else{
    query.append("INSERT INTO users("
                    "name,"
                    "surname,"
                    "age,"
                    "class)"
                    "VALUES("
                    "'"+ui->lineEditName->text()+"',"
                    "'"+ui->lineEditSurname->text()+"',"
                    ""+ui->lineEditAge->text()+","
                    ""+ui->lineEditClass->text()+""
                    ");");
    }

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
    ui->lineEditName->setText(temp.toLatin1());
   // insertUser();
   // selectUsers();

    createyearly(temp.toLatin1());
   // createyearly("FvEZ0TCH4YOVaaaaaaaaaaaaaaaaaaaaaaaaaaa");
   // createyearly("FvEZ0TCH4YOV");
    //selectUsersCoins(temp.toLatin1(),year.toLatin1());

    //combine user year+userid to give to user


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

void MainWindow::placeCoins() //free coins from coins.db
{
// encrypt coin based on index and client encryption key or master encryption key dual encryption probably not required to speed things up might
    //even be able to use xor and hash.
// place into client walled based on yearly tables when created ID

    //https://doc.qt.io/qt-5/qcryptographichash.html
//    QString test=md5Checksum("testing123");//
//    qDebug() << "md5sum:" << test.toLatin1();

//    QByteArray array;
//    array.append("ABCDEF12343");
//    qDebug() << QString(array.toHex());

    //    QString resultxor = XORencryptDecrypt("testing", "key2");
    //       qDebug() <<"xor:"<<resultxor ;
    //    resultxor = XORencryptDecrypt(resultxor.toLatin1(), "key2");
    //            qDebug() <<"xor:"<<resultxor ;

    //            string test = XOR("testing", "key2");
    //                    qDebug() <<"xor:"<<test.c_str() ;
                       // string XOR(test.c_str(), "key2");
                       // qDebug() <<"xor:"<<test.c_str() ;

   // ui->givecoinsid.text().toLatin1()
//ui->givecoinsammount.text().toLatin1()
    //pull from rcoins db and remove coin after placing in tmp text file

//    db.setDatabaseName("rcoins.sqlite");
//    db.open();
//        QSqlDatabase::database().transaction();
//        QSqlQuery query;
//        query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
//        if (query.next()) {
    //removecoins
//            int employeeId = query.value(0).toInt();
//            query.exec("INSERT INTO project (id, name, ownerid) "
//                       "VALUES (201, 'Manhattan Project', "
//                       + QString::number(employeeId) + ')');
    //place into textfile
//        }
//        QSqlDatabase::database().commit();
//    db.close();


//verify coins and insert into yearly userid
//QString decrypt = encryptxor("test","key").toLatin1();
//    qDebug() << decrypt;
//  //  qDebug() <<XOR2 (test2.toStdString(),"tring");
//    qDebug() << decryptxor(decrypt,"key");

//    db.setDatabaseName("database.sqlite");
//    db.open();
//        QSqlDatabase::database().transaction();
//        QSqlQuery query2;
//        query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
//        if (query.next()) {
//            int employeeId = query.value(0).toInt();
//            query.exec("INSERT INTO project (id, name, ownerid) "
//                       "VALUES (201, 'Manhattan Project', "
//                       + QString::number(employeeId) + ')');
//        }
//        QSqlDatabase::database().commit();
//    db.close();
}

void MainWindow::on_placeCoins_clicked()
{
    //decrypt and verify coins / rot13 or xor should suffice to keep the addresses smaller.
    //if any incorrect flag account for checking also disable other transactions.
    int verified = 0;//md5verifydb();

    placeCoins();

    if (verified == 1){
    QMessageBox Msgbox;
        Msgbox.setText("coins sent ");
        Msgbox.exec();
    }
}

void MainWindow::on_SendCoins_clicked()
{
    //check for master keys from usb drive
    //check available coins has enough for tx
    //check ammount is proper format
    //xor coins with user account id or rot13 and place into user account

    //ui->receiveid.text().toLatin1()
    //ui->receiveammount.text().toLatin1()
    //    QString crypted = simplecrypt("test","test2");
//    qDebug() << crypted;
//    QString decrypted = simpledecrypt(crypted,"test2");
//    qDebug() << decrypted;

//    //remove coins from userindex
//    //find user in yearly db pull coins out and verify validity then place back into rcoins
//    //re-md5sum file
//    db.setDatabaseName("rcoins.sqlite");
//    db.open();
//        QSqlDatabase::database().transaction();
//        QSqlQuery query;
//        query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
//        if (query.next()) {
//            int employeeId = query.value(0).toInt();
//            query.exec("INSERT INTO project (id, name, ownerid) "
//                       "VALUES (201, 'Manhattan Project', "
//                       + QString::number(employeeId) + ')');
//        }
//        QSqlDatabase::database().commit();
//    db.close();


//QString decrypt = encryptxor("test","key").toLatin1();
//    qDebug() << decrypt;
//  //  qDebug() <<XOR2 (test2.toStdString(),"tring");
//    qDebug() << decryptxor(decrypt,"key");

//    db.setDatabaseName("database.sqlite");
//    db.open();
//        QSqlDatabase::database().transaction();
//        QSqlQuery query2;
//        query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
//        if (query.next()) {
//            int employeeId = query.value(0).toInt();
//            query.exec("INSERT INTO project (id, name, ownerid) "
//                       "VALUES (201, 'Manhattan Project', "
//                       + QString::number(employeeId) + ')');
//        }
//        QSqlDatabase::database().commit();
//    db.close();


    //placeCoins();
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
