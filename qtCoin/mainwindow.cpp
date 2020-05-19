#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/downloadmanager.h"
#include "src/loadtheme.h"
#include <QFileDialog>
#include <src/coingenerator.h>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <src/coinfunctions.h>
#include <src/validatecoins.h>
#include "./src/smtp/SmtpMime"

//#include <QtCore>
//references and links
//https://doc.qt.io/qt-5/sql-sqlstatements.html
//https://www.techonthenet.com/mysql/select.php
//http://ismacs.net/singer_sewing_machine_company/why-two-serial-numbers.html some of the first serial numbers
//https://patents.google.com/patent/US3988571A/en

//encrypt with usbdrivename

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    version=0.6;

    ui->setupUi(this);
    qDebug()<<"Application initialized...";

    db = QSqlDatabase::addDatabase("QSQLITE");    
    db.setDatabaseName("database.sqlite");

    player=new QMediaPlayer();
    playsound("Resource/sounds/ec1_mono.ogg");

    qDebug() << "db exists" << QDir("db").exists();    if (!QDir("db").exists()){        QDir().mkdir("db");    }

    ui->createyear->setValue(year.toInt());
    ui->createmonth->setCurrentIndex(QDate::currentDate().month()-1);
    ui->maturemonth->setCurrentIndex(QDate::currentDate().month()-1);
    QTime starttime(QTime::currentTime().hour(),QTime::currentTime().minute()); //12:00AM
    ui->createtime->setTime(starttime);

    //load settings
    QFile Fout("settings.txt");    if(Fout.exists())    {        on_actionOpenCoin_triggered();    }    Fout.close();

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
    ui->progress->setValue( test4);

    //enable these if you want to test rsa+aes encryption in the onpushbutton_3_clicked function.
//    rsaTester = new Rsa();
//    rsaTester->publish_keys(m_e, m_n);

    if(getkeys() == 1){
      //
    }else{ //testing
        QString tester1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
        masterkey = GetRandomString(12,tester1.toLatin1());
        coinkey = "testing1234567";
    }

    //if client only mode

//ui->settingstab->setEnabled(false);
//ui->settingstab->setVisible(false);
//   QWidget * test= ui->app->widget(2);
//ui->app->removeTab(2);
//ui->app->insertTab(2,test,"Settings");

  //  ui->createtime->setTime(starttime);

}


MainWindow::~MainWindow()
{
    delete ui;
    //QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
}

void MainWindow::playsound(QString test){
    // player->setMedia(QUrl("qrc:/sounds/ec1_mono.ogg"));
    // player->setMedia(QUrl::fromLocalFile("./paddle_hit.wav"));
     //or play from memory
      QFile file(test.toLatin1());
      file.open(QIODevice::ReadOnly);
      QByteArray* arr = new QByteArray(file.readAll());
      file.close();
      QBuffer* buffer = new QBuffer(arr);
      buffer->open(QIODevice::ReadOnly);
      buffer->seek(0);

      player->setVolume(10);
 //    media->setMedia("sound.mp3");
     player->setMedia(QMediaContent(), buffer);
     player->play();
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

void MainWindow::serverusbtxrx(){
    //automatic function to do rxtx from usb for cold storage
    //export db's and overwrite if valid

}

void MainWindow::clientusbtxrx(){
    //import db's and overwrite if valid

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

//                qDebug() << "name:" << storage.name();
//                qDebug() << "fileSystemType:" << storage.fileSystemType();
//                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
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


void MainWindow::on_placeCoins_clicked()
{
    //if any incorrect flag account for checking also disable other transactions.
    int verified = 0;//md5verifydb();



    placeCoins("receive",ui->receiveammount->text().toLatin1());

    if (verified == 1){
    QMessageBox Msgbox;
        Msgbox.setText("coins sent ");
        Msgbox.exec();
    }
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

void MainWindow::on_test_clicked()
{
    QString test = rot13("test");
    qDebug() << "encrypted rot13:" << test;
    qDebug() << "decrypted rot13:" << rot13(test.toLatin1());
}

int MainWindow::smtpsend(QString toemail,QString Message){

    bool ssl;
    if ( ui->smtpssl->isChecked() == 1 ){ ssl = 1;    }else{ ssl = 0; }

    SmtpClient smtp (ui->smtphost->text().toLatin1(), ui->smtpport->text().toInt(), ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);
    smtp.setUser(ui->smtpemail->text());
    smtp.setPassword(ui->smtppassword->text());

    MimeMessage message;
    EmailAddress sender(ui->smtpemail->text(), ui->smtpemail->text()); //email,name
    message.setSender(&sender);
    EmailAddress to(toemail.toLatin1(), toemail.toLatin1()); // email, name
    message.addRecipient(&to);
    message.setSubject(ui->coinname->text());

    MimeText text;
  //  text.setText("Hi,\nThis is a simple email message.\n");
    text.setText(Message.toLatin1());
    // Now add it to the mail
    message.addPart(&text);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
        return 1;
    }

    if (!smtp.login()) {
        qDebug() << "Failed to login!" << endl;
        return 2;
    }

    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;
        return 3;
    }
    smtp.quit();
}

void MainWindow::on_smtpsave_clicked()
{

}

void MainWindow::on_smtptestmessage_clicked()
{

}


void MainWindow::on_saveuserinfo_clicked()
{

}

void MainWindow::on_placeCoinsopenfile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.rx *.tx *.txt)"));
    qDebug()<< fileName.toLatin1() ;

    processRXTXfile(fileName);
}



