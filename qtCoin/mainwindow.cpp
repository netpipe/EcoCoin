#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/qstylesheetmanager.h"
#include "src/downloadmanager.h"
#include <QFileDialog>
#include <coingenerator.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"Application initialized...";

    db = QSqlDatabase::addDatabase("QSQLITE");    
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {       qDebug()<<"Successful database connection";    }
    else    {       qDebug()<<"Error: failed database connection";    }
  //  if ( db.isOpen() )
  //      std::cout << "the database is:  open" << std::endl;
    db.close();

    createUserTable();
    selectUsers();


 //   coinDB = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("coins.sqlite");
    if(db.open())
    {        qDebug()<<"Successful coin database connection";    }
    else
    {        qDebug()<<"Error: failed database connection";    }
    db.close();

    createCoinTable();

//QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );

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
  //  player->play();

//    QString s = QDate::currentDate().toString();
//    QDate::currentDate().day();
//    QDate::currentDate().month();
//    QDate::currentDate().year();

    ui->progress->setValue(50);

    QFile Fout("settings.txt");
    if(Fout.exists())
    {
        on_actionOpenCoin_triggered();
    }
    Fout.close();

    themeInit();

    int yearvar= QDate::currentDate().year();
    year = QString::number(yearvar);
    QString temp = GenerateClientAddress(12);
  //  qDebug() << QDate::currentDate().year();
    qDebug() <<temp ;

    rsaTester = new Rsa();
    rsaTester->publish_keys(m_e, m_n);



}


MainWindow::~MainWindow()
{
    delete ui;
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
       usbpath = storage.rootPath().contains(usbstring);
  if (storage.rootPath().contains(usbstring)){
qDebug() << "yep" << "/n";
    }

       if (storage.isReadOnly())
           qDebug() << "isReadOnly:" << storage.isReadOnly();

       qDebug() << "name:" << storage.name();
       qDebug() << "fileSystemType:" << storage.fileSystemType();
       qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
       qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
    }
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

void MainWindow::createCoinTable()
{
    //db.setDatabaseName("coins.sqlite");
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
    query.append("CREATE TABLE IF NOT EXISTS coins("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "addr VARCHAR(50));");

    //SELECT column FROM table
    //ORDER BY RAND()
    //LIMIT 1

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



void MainWindow::insertUser()
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
                    "name,"
                    "surname,"
                    "age,"
                    "class)"
                    "VALUES("
                    "'"+bFname+"',"
                    "'"+ui->lineEditSurname->text()+"',"
                    ""+ui->lineEditAge->text()+","
                    ""+ui->lineEditClass->text()+""
                    ");");

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
    QString temp = GenerateClientAddress(12);
    ui->lineEditName->setText(temp.toLatin1());
    insertUser();
    selectUsers();


}

void MainWindow::on_gencoininfo_btn_clicked()
{
    QFile file("settings.txt");
      //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          if(file.open(QIODevice::ReadWrite | QIODevice::Text))
          {
              QTextStream stream(&file);
              stream << "CoinName:" << ui->coinname->text() <<'\n';
              stream << "CoinCount:" << ui->coincount->text() <<'\n';
              stream << "CoinLength:" << ui->coinlength->text() <<'\n';
              stream << "createday:" << ui->createday->text() <<'\n';
              stream << "CreateMonth: " << ui->createmonth->currentText() <<'\n';
              stream << "createyear:" << ui->createyear->text() <<'\n';
              stream << "createtime:" << ui->createtime->time().hour() << "|" << ui->createtime->time().minute()  <<'\n';
              stream << "coinvalue:" << ui->coinvalue->text() <<'\n';
              stream << "matures:" << ui->matureradio_yes->text() <<'\n';
              stream << "coinpayout:" << ui->coinpayout->text() <<'\n';
              stream << "encrypted:" << ui->encrypted_yes->text() <<'\n';
              stream << "maturedate:" << ui->matureday->text() <<'\n';
              stream << "maturemonth:" << ui->maturemonth->currentText() <<'\n';
              stream << "matureyear:" << ui->matureyear->text() <<'\n';
              stream << "maturetime:" << ui->maturetime->time().hour() << "|" << ui->maturetime->time().minute() <<'\n';
              stream << "maturedescription:" << ui->maturedescription->toPlainText()<<'\n';
              stream << "usbdrivename:" << ui->usbdrivename->text();
              file.close();
          }
}

void MainWindow::on_actionOpenCoin_triggered()
{//https://stackoverflow.com/questions/31384273/how-to-search-for-a-string-in-a-text-file-using-qt
    QString searchString(":");
    QFile MyFile("settings.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
  //  int ii=0;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;


    do {
        line = in.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());

                QString coinname=nums.at(0);
                ui->coinname->setText(coinname.toLatin1());
                    qDebug("%s", qUtf8Printable(coinname));
                QString coincount=nums.at(1);
                ui->coincount->setText(coincount.toLatin1());
                    qDebug("%s", qUtf8Printable(coincount));
                //  QString coincount=nums.at(0).toLocal8Bit().constData();
                QString CoinLength=nums.at(2);
                ui->coinlength->setText(CoinLength.toLatin1());
                qDebug("%s", qUtf8Printable(CoinLength));
                QString createday=nums.at(3);
             //   ui->createday->value(createday.toInt());
                qDebug("%s", qUtf8Printable(createday));
                QString CreateMonth=nums.at(4);
                //ui->createmonth->setValue();comboBox->currentIndex();
                qDebug("%s", qUtf8Printable(CreateMonth));
                QString createyear=nums.at(5);
                ui->createyear->setValue(createyear.toInt());
                qDebug("%s", qUtf8Printable(createyear));

                QString createtime=nums.at(6);
                QRegExp rx("[|]");// match a comma or a space
                QStringList list2 = createtime.split(rx);
//                QString ampm = list2.at(1);
//                QRegExp rx2("[A]");// match a comma or a space
//                QStringList list4 = ampm.split(rx2);
                //qDebug("test %s", qUtf8Printable(list4.at(0).toInt()));
//                if(regex.exactMatch(value))
//                {
//                   //If regex does match - Doesn't work!
//                }
                //QTime time(list2.at(0).toInt(),list4.at(0).toInt()); //12:00AM
                QTime time(list2.at(0).toInt(),list2.at(1).toInt()); //12:00AM

                ui->createtime->setTime(time);

                    qDebug("%s", qUtf8Printable(createtime));
                QString coinvalue=nums.at(7);
                ui->coinvalue->setText(coinvalue.toLatin1());
                    qDebug("%s", qUtf8Printable(coinvalue));
                QString matures=nums.at(8);
              //  ui->matureradio_yes->setEnabled(matures.toInt());
                if ( matures.toLatin1() == "Yes"){
                    ui->matureradio_yes->setChecked(1);
                    ui->matureradio_no->setChecked(0);

                }else{
                    ui->matureradio_yes->setChecked(0);
                    ui->matureradio_no->setChecked(1);
                }
                    qDebug("%s", qUtf8Printable(matures));
                QString coinpayout=nums.at(9);
                ui->coinpayout->setValue(coinpayout.toInt());
                    qDebug("%s", qUtf8Printable(coinpayout));
                QString encrypted=nums.at(10);
               // ui->encrypted_yes->text();

                if ( encrypted.toLatin1() == "Yes"){
                    ui->encrypted_yes->setChecked(1);
                    ui->encrypted_no->setChecked(0);

                }else{
                    ui->encrypted_yes->setChecked(0);
                    ui->encrypted_no->setChecked(1);
                }
                    qDebug("%s", qUtf8Printable(coinpayout));
                QString maturedate=nums.at(11);
                ui->matureday->setValue(maturedate.toInt());
                    qDebug("%s", qUtf8Printable(maturedate));
                QString maturemonth=nums.at(12);
             //   ui->maturemonth->se(maturemonth.);
                    qDebug("%s", qUtf8Printable(maturemonth));
                QString matureyear=nums.at(13);
                ui->matureyear->text();
                    qDebug("%s", qUtf8Printable(matureyear));

                QString maturetime=nums.at(14);
                 list2 = maturetime.split(rx);
//                 ampm = list2.at(1);
//                 list4 = ampm.split(rx2);
                   // qDebug("test %s", qUtf8Printable(list4.at(0).toInt()));
//                if(regex.exactMatch(value))
//                {
//                   //If regex does match - Doesn't work!
//                }
                 //QTime time2(list2.at(0).toInt(),list4.at(0).toInt()); //12:00AM
                 QTime time2(list2.at(0).toInt(),list2.at(1).toInt()); //12:00AM
                 ui->maturetime->setTime(time2);
                // ui->maturetime->setDate();
                    qDebug("%s", qUtf8Printable(maturetime));
                 QString maturedescription=nums.at(15);
                 ui->maturedescription->toPlainText();
                    qDebug("%s", qUtf8Printable(maturedescription));
                 QString usbdrivename=nums.at(16);
                 ui->usbdrivename->setText("usbdrivename");
                    qDebug("%s", qUtf8Printable(usbdrivename));
}

void MainWindow::on_pushButton_clicked() //generate coins button
{
      coini=0;
    GenerateCoins3(ui->coinlength->text().toInt(),ui->coincount->text().toInt());
    RandomizeCoins();
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

void MainWindow::themeInit(){



    QDir directory("./");
    QStringList themes = directory.entryList(QStringList() << "*.qss" << "*.qss",QDir::Files);

    for (int i=0;i < themes.size(); i++){
        QString themetmp = themes.at(i);
        ui->cmbTheme->addItem((themetmp.toLatin1() ));
        qDebug()<<themetmp << "/n";
    }

    //        QFile file("themes.txt");
    //        if (file.open(QIODevice::ReadOnly))
    //        {
    //           QTextStream stream(&file);
    //           QString entry = stream.readLine();
    //           while (!(stream.atEnd()))
    //           {
    //              ui->cmbTheme->addItem(entry);
    //              entry = stream.readLine();
    //           }
    //           ui->cmbTheme->addItem(entry);
    //           file.close();
    //        }

    //    ui->cmbTheme->itemText(ui->cmbTheme->count());

    if (ui->cmbTheme->currentText().toLatin1() != ""){
        //QStyleSheetManager::loadStyleSheet( ui->cmbTheme->currentText().toLatin1());
        QStyleSheetManager::loadStyleSheet(  ui->cmbTheme->itemText(ui->cmbTheme->count()-1));
    }

    QFile file("themes.txt");
        if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
        {
                QTextStream stream(&file);
                for (int i = 0; i < ui->cmbTheme->count(); i++)
                {
                 stream << ui->cmbTheme->itemText(i) << endl;
                }
            //                file.write("\n");
               file.close();
        }
}

void MainWindow::on_btnAddThemeFromFile_clicked()
{
    QString newFile = QFileDialog ::getOpenFileName(0,"Select File","","Files (*.qss)");
    ui->cmbTheme->addItem(newFile);

    QFile file("themes.txt");
        if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
        {
                QTextStream stream(&file);
                for (int i = 0; i < ui->cmbTheme->count(); i++)
                {
                 stream << ui->cmbTheme->itemText(i) << endl;
                }
            //                file.write("\n");
               file.close();
        }
}

void MainWindow::on_btnRemoveThemeFromFile_clicked()
{
    ui->cmbTheme->removeItem(ui->cmbTheme->currentIndex());
    QFile file("themes.txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
    {
        QTextStream stream(&file);
        for (int i = 0; i < ui->cmbTheme->count(); i++)
        {
            stream << ui->cmbTheme->itemText(i) << endl;
        }
     //file.write("\n");
     file.close();
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
// validate first ? (depends on speed)
// encrypt coin based on index and client encryption key or master encryption key dual encryption probably not required to speed things up might even be able to use xor and hash.
// place into client walled based on yearly tables

    //https://doc.qt.io/qt-5/qcryptographichash.html
QString test=md5Checksum("testing123");//
qDebug() << "md5sum:" << test.toLatin1();

    QByteArray array;
    array.append("ABCDEF12343");
    qDebug() << QString(array.toHex());


    //    QString resultxor = XORencryptDecrypt("testing", "key2");
    //       qDebug() <<"xor:"<<resultxor ;
    //    resultxor = XORencryptDecrypt(resultxor.toLatin1(), "key2");
    //            qDebug() <<"xor:"<<resultxor ;

    //            string test = XOR("testing", "key2");
    //                    qDebug() <<"xor:"<<test.c_str() ;
                       // string XOR(test.c_str(), "key2");
                       // qDebug() <<"xor:"<<test.c_str() ;

}

void MainWindow::md5verifydb(){
// md5sum coinsdb maybe choose semirandom coinsammount to make md5 more unique
// save with settings.txt
//verify databases

    //md5 convert coinsdb to randomcoins.db then md5sum can also check freecoins.db after each tx
}

void MainWindow::on_placeCoins_clicked()
{
    placeCoins();
}

void MainWindow::on_SendCoins_clicked()
{
    simplecrypttest();
    //placeCoins();
}
